/* serial.ino
 * ~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * ----------------------------------------------------------------------------
 *
 * Description
 * ~~~~~~~~~~~
 * - None, yet.
 *
 * To Do
 * ~~~~~
 * - Get Serial.begin(DEFAULT_BAUD_RATE) from EEPROM.
 * - Use #define for "[aiko_mqtt_lcd 0.0]".
 * - Use PROGMEM for literal strings.
 * - Refactor EEPROM loader readMessage() to be independent of Serial functions.
 * - Break-out Keyboard simulator and EEPROM loader into separate .ino files.
 * - Or combine Keyboard simulator and EEPROM loader ...
 *   - Refactor Serial functions: Provide available(), read() and pushback().
 * - Break-out readMessage() and parseMessage() into separate .ino files.  // AAG
 * - Consolidate with existing Aiko serial command handling / parsing.
 */

// Only one of these can be defined at a time
// #define KEYBOARD_SIMULATOR
#define EEPROM_LOADER

/* ------------------------------------------------------------------------- */

#ifdef KEYBOARD_SIMULATOR

struct keyEventType {
  byte key;
  void (*eventHandler)(void);
};

static const struct keyEventType keyEventMap[] = {
  'h', buttonLeftEvent,
  'l', buttonRightEvent,
  'k', buttonUpEvent,
  'j', buttonDownEvent,
  'y', buttonSelectEvent
};

static const byte KEY_EVENT_COUNT = sizeof(keyEventMap) / sizeof(keyEventType);

void serialWelcome(void) {
  Serial.println("[aiko_mqtt_lcd 0.0] Keyboard simulator");
}

void serialInputHandler(void) {
  while (Serial.available()) {
    byte key = Serial.read();

    for (byte index = 0;  index < KEY_EVENT_COUNT;  index ++) {
      if (keyEventMap[index].key == key) (* keyEventMap[index].eventHandler)();
    }
  }
}
#endif

/* ------------------------------------------------------------------------- */

#ifdef EEPROM_LOADER

#define ERROR_TIMEOUT      "(error timeout)"
#define ERROR_BAD_MESSAGE  "(error bad-message)"
#define ERROR_BAD_COMMAND  "(error bad-command)"
#define ERROR_BAD_ARGUMENT "(error bad-argument)"

static const byte  NUL   = 0x00;  // Null character
static const byte  STX   = 0x02;  // Start of TeXt
static const byte  ETX   = 0x03;  // End of TeXt
static const byte  HT    = 0x09;  // Horizontal Tab
static const byte  LF    = 0x10;  // Line Feed
static const byte  CR    = 0x13;  // Carriage Return
static const byte  SPACE = 0x20;  // Space bar
static const byte  LBRAC = 0x28;  // Left bracket '('
static const byte  RBRAC = 0x29;  // Right bracket ')'

static const byte MESSAGE_MAXIMUM = 32;
static const int  MESSAGE_TIMEOUT = 5000;  // milliseconds

unsigned long messageTimer;
char message[MESSAGE_MAXIMUM];
byte messageLength = 0;
byte messageIndex;

void (*commandHandlers[])() = {
  commandDelete,
  commandDump,
  commandGet,
  commandLock,
  commandPut,
  commandReset,
  commandSetup,
  commandUnlock,
  commandVerify,
  commandWipe
};

char* commands[] = {
  "delete",
  "dump",
  "get",
  "lock",
  "put",
  "reset",
  "setup",
  "unlock",
  "verify",
  "wipe"
};

byte commandCount = sizeof(commands) / sizeof(*commands);

byte argumentsCount[] = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
byte argumentIndex;

void serialWelcome(void) {
  Serial.println("[aiko_mqtt_lcd 0.0] EEPROM loader");
  Serial.print("Free RAM: ");
  Serial.println(freeRAM());   // AAG: Recover RAM, use PROGMEM for Serial.print("literal")
}

void serialInputHandler(void) {
  unsigned long timeNow = millis();

  if (messageLength > 0) {
    if (timeNow >= messageTimer) replyError(ERROR_TIMEOUT);
  }

  if (readMessage()) parseMessage();
}

boolean readMessage() {
  while (Serial.available()) {
    messageTimer = millis() + MESSAGE_TIMEOUT;

    char data = Serial.read();
    
    switch(data) {
      case STX:
      case ETX:
        messageLength = 0;
        break;

      case HT:
      case LF:
      case CR:
        data = SPACE;

      case SPACE:
        if (messageLength < 1) break;
        if (message[messageLength - 1] == SPACE) break;    // reduce whitespace
                                                                
      default:
        if (messageLength >= sizeof(message)) {
          replyError(ERROR_BAD_MESSAGE);
        }
        else {
          message[messageLength ++] = data;
          
          if (data == RBRAC) return(true);  // complete message ready
        }
        break;
    }
  }

  return(false);  // partial message only
}

void parseMessage() {
  messageIndex = 0;
  
  skipWhitespace();  // updates messageIndex

  if (checkForCharacter(LBRAC) == false) {
    replyError(ERROR_BAD_MESSAGE);  // no leading '('
  }
  else {
    skipWhitespace();  // updates messageIndex

    byte command = parseCommand();
    
    if (command < 0) {
      replyError(ERROR_BAD_COMMAND);  // invalid command
    }
    else {
      skipWhitespace();  // updates messageIndex

      if (parseArguments(argumentsCount[command]) == false) {
        replyError(ERROR_BAD_ARGUMENT);  // invalid arguments
      }
      else {
        skipWhitespace();  // updates messageIndex

        if (checkForCharacter(RBRAC) == false) {
          replyError(ERROR_BAD_MESSAGE);  // no trailing ')'
        }
        else {
          (commandHandlers[command])();  // execute command
        }
      }
    }
  }

  messageLength = 0;
}

void skipWhitespace() {
  while (messageIndex < messageLength  &&  message[messageIndex] == SPACE) {
    messageIndex ++;
  }
}

boolean checkForCharacter(
  char expected) {
  
  if (messageIndex < messageLength) {
    if (message[messageIndex] == expected) {
      messageIndex ++;
      return(true);  // expected character found
    }
  }
  
  return(false);  // expected character not found
}

byte parseCommand() {
  byte commandIndex = messageIndex;

  parseToken();  // updates messageIndex
  
  for (byte command = 0; command < commandCount; command ++) {
    if (stringCompare(commands[command], & message[commandIndex])) {
      return(command);  // valid command found
    }
  }

  return(-1);  // invalid command
}

boolean parseArguments(
  byte expectedArgumentCount) {

  argumentIndex = messageIndex;

  byte argumentCount = 0;
  
  while (parseToken() > 0) {  // updates messageIndex
    argumentCount ++;
    
    skipWhitespace();
  }
  
  if (argumentCount == 0) argumentIndex = -1;
    
  return(expectedArgumentCount == argumentCount);    // valid argument count ?
}

byte parseToken() {
  byte tokenIndex = messageIndex;

  while (messageIndex < messageLength) {
    if (stringDelimiter(message[messageIndex])) break;
    messageIndex ++;
  }
  
  return(messageIndex - tokenIndex);  // token length
}

boolean stringCompare(
  char source[],
  char target[]) {

  byte index = 0;

  while (stringDelimiter(source[index]) == false  ||
         stringDelimiter(target[index]) == false) {
    
    if (source[index] != target[index]) return(false);
    
    index ++;
  }

  return(true);
}

boolean stringDelimiter(
  char data) {
  
  return(data == NULL  ||  data == SPACE  ||  data == RBRAC);
}
void replyError(
  char *errorMessage) {

  messageLength = 0;
  Serial.println(errorMessage);
}
#endif

/* ------------------------------------------------------------------------- */

byte serialInitialized = false;

void serialInitialize(void) {
  if (! serialInitialized) {
    Serial.begin(DEFAULT_BAUD_RATE);
    serialWelcome();

    serialInitialized = true;
  }
}

void serialHandler(void) {
  if (! serialInitialized) serialInitialize();

  serialInputHandler();
}

/* ------------------------------------------------------------------------- */
