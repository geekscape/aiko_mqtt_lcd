/* button.ino
 * ~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * ----------------------------------------------------------------------------
 *
 * Description
 * ~~~~~~~~~~~
 * Turn analog input for button resistor ladder into a "buttonIndex".
 * When the button is pressed down, fire an associated event.
 *
 * To Do
 * ~~~~~
 * - Holding button down causes repetitive event generation.
 * - Holding button down for a long time performs some action, e.g preferences.
 */

struct buttonEventType {
  int  lowWaterMark;
  int  highWaterMark;
  void (*eventHandler)(void);
};

// Analog input button voltage ladder values for Freetronics LCD shield
// http://www.freetronics.com/products/lcd-keypad-shield

static const struct buttonEventType buttonEventMap[] = {
  1023, 1023, NULL,               // No button
   731,  751, buttonSelectEvent,  // Normal:  741
   493,  513, buttonLeftEvent,    // Normal:  503
   317,  337, buttonDownEvent,    // Normal:  327
   132,  152, buttonUpEvent,      // Normal:  142
     0,   10, buttonRightEvent    // Normal:    0
};

static const byte BUTTON_EVENT_COUNT =
  sizeof(buttonEventMap)/sizeof(buttonEventType);

byte buttonIndex = 0;                         // Current button pressed or 0

byte buttonInitialized = false;

void buttonInitialize(void) {
  if (! buttonInitialized) {
    digitalWrite(PIN_BUTTONS, LOW);           // Ensure internal pull-up is off

    buttonInitialized = true;
  }
}

void buttonHandler(void) {
  if (! buttonInitialized) buttonInitialize();

  int  value = analogRead(PIN_BUTTONS);
  byte newIndex = 0;

  for (byte index = 0;  index < BUTTON_EVENT_COUNT;  index ++) {
    if (buttonEventMap[index].lowWaterMark  <= value  &&
        buttonEventMap[index].highWaterMark >= value) {

      newIndex = index;
    }
  }

  if (newIndex != buttonIndex) {
    buttonIndex = newIndex;

    if (buttonIndex > 0) (* buttonEventMap[buttonIndex].eventHandler)();
  }
}

/* ------------------------------------------------------------------------- */
