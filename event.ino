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
 * - Comment-out serial console debugging diagnostics, when finished.
 */

void eventLog(
  char message[]) {

  Serial.println(message);
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
