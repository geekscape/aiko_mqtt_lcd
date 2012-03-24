/* eeprom.ino
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
 * - Document EEPROM layout.
 * - #define for different Arduino EEPROM sizes.
 * - Use EEPROM struct for magic number, length, Ethernet, MQTT and checksum ?
 * - Commands ...
 *   - (reset)         - Reboot.
 *   - (clear)         - Initialize EEPROM.
 *   - (verify)        - Verify EEPROM.
 *   - (get key)       - Return value from EEPROM, key can be "all".
 *   - (put key value) - Store value in EEPROM.
 *   - (delete key)    - Remove value from EEPROM.
 *   - (lock secret)   - Prevent system control of Arduino.
 *   - (unlock secret) - Allow system control of Arduino.
 * - Store ...
 *   - Ethernet: Mac address, IP address, network mask, gateway address.
 *   - MQTT server: Host address, port, client id
 *   - Topic display: screen id, LCD row, LCD column, length, topic
 *   - Static label:  screen id, LCD row, LCD column, length
 *   - Serial baud rate.
 *   - LCD update rate, pin initialization parameters ?
 */

byte eepromInitialized = false;

void eepromInitialize(void) {
  eepromInitialized = true;
}

void eepromHandler(void) {
  if (! eepromInitialized) eepromInitialize();
}

void commandClear(void) {
  eventLog("clear()");
}

void commandDelete(void) {
  eventLog("delete()");
}

void commandGet(void) {
  eventLog("get()");
}

void commandLock(void) {
  eventLog("lock()");
}

void commandPut(void) {
  eventLog("put()");
}

void commandReset(void) {
  eventLog("reset()");
}

void commandUnlock(void) {
  eventLog("unlock()");
}

void commandVerify(void) {
  eventLog("verify()");
}

/* ------------------------------------------------------------------------- */
