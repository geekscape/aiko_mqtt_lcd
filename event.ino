/* event.ino
 * ~~~~~~~~~
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
 * - Event should publish to MQTT topic (stored in EEPROM).
 * - Comment-out serial console debugging diagnostics, when finished.
 */

byte eventInitialized = false;

void eventInitialize(void) {
  if (! eventInitialized) {
    lcdInitialize();
    serialInitialize();

    eventInitialized = true;
  }
}

void eventLog(
  char message[]) {

  if (! eventInitialized) eventInitialize();

  Serial.println(message);  // AAG: Sent to MQTT if connected !

  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print(message);

  lcdClearFieldCounter = secondCounter + 2;
}

void buttonDownEvent(void) {
  eventLog("Down");

//changeScreen(1);
}

void buttonLeftEvent(void) {
  eventLog("Left");
}

void buttonRightEvent(void) {
  eventLog("Right");
}

void buttonSelectEvent(void) {
  eventLog("Select");
}

void buttonUpEvent(void) {
  eventLog("Up");

//changeScreen(-1);
}

/* ------------------------------------------------------------------------- */
