/* aiko_mqtt_lcd.ino
 * ~~~~~~~~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * Version: 0.0
 *
 * Documentation
 * ~~~~~~~~~~~~~
 * Software: http://geekscape.github.com/aiko_mqtt_lcd
 * Hardware  http://freetronics.com/products/lcd-keypad-shield
 * ----------------------------------------------------------------------------
 *
 * Third-Party libraries
 * ~~~~~~~~~~~~~~~~~~~~~
 * These libraries are not included in the Arduino IDE and
 * need to be downloaded and installed separately.
 *
 * - Aiko framework
 *   https://github.com/geekscape/aiko_arduino
 *
 * Arduino MQTT client robustness
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * - Use updated Arduino MQTT client ...
 *   - http://knolleary.net/2012/03/08/updated-arduino-mqtt-client-4
 * - http://knolleary.net/2011/05/25/staying-connected
 * - https://gist.github.com/900885
 *   - Uniqueness of all addresses and identifiers per Arduino
 *   - Ethernet controller reset code and re-leasing of addresses ...
 *     in setup() and whenever MQTT disconnection occurs
 *   - Use of “last will and testament” in Arduino client code and Lua scripts
 *
 * To Do
 * ~~~~~
 * - Extract common Event, MQTT, Serial code out of application and into Aikoframework.
 * - Events.addHandler(lcdHandler, rate) ... rate loaded from EEPROM ?
 */

#include <AikoEvents.h>

using namespace Aiko;

#include "aiko_mqtt_lcd.h"

void setup(void) {
  eepromInitialize();

  Events.addHandler(buttonHandler,    100);
  Events.addHandler(clockHandler,    1000);
  Events.addHandler(lcdHandler,       100);
  Events.addHandler(throbberHandler,  250);
  Events.addHandler(mqttHandler,      250);                // ToDo: Adjust timing
  Events.addHandler(serialHandler,    100);
}

void loop(void) {
  Events.loop();
}

/* ------------------------------------------------------------------------- */

unsigned long secondCounter = 0;

void clockHandler(void) {
  ++ secondCounter;
}

/* ------------------------------------------------------------------------- */
// http://jeelabs.org/2011/05/22/atmega-memory-use

int freeRAM(void) {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

/* ------------------------------------------------------------------------- */
