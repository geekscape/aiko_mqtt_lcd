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
 *   - (delete key)    - Remove value from EEPROM.
 *   - (get key)       - Return value from EEPROM, key can be "all".
 *   - (lock secret)   - Prevent system control of Arduino.
 *   - (put key value) - Store value in EEPROM.
 *   - (reset)         - Reboot.
 *   - (setup)         - Initialize EEPROM keys / values.
 *   - (unlock secret) - Allow system control of Arduino.
 *   - (verify)        - Verify EEPROM.
 * - Store ...
 *   - Ethernet: Mac address, IP address, network mask, gateway address.
 *   - MQTT server: Host address, port, client id
 *   - Topic display: screen id, LCD row, LCD column, length, topic
 *   - Static label:  screen id, LCD row, LCD column, length
 *   - Serial baud rate.
 *   - LCD update rate, pin initialization parameters ?
 */

#include <avr/eeprom.h>

// EEPROM macros: http://projectgus.com/2010/07/eeprom-access-with-arduino

#define eeprom_read_to(dst_p, eeprom_field, dst_size) eeprom_read_block(dst_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(dst_size, sizeof((__eeprom_data*)0)->eeprom_field))

#define eeprom_read(dst, eeprom_field) eeprom_read_to(&dst, eeprom_field, sizeof(dst))

#define eeprom_write_from(src_p, eeprom_field, src_size) eeprom_write_block(src_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(src_size, sizeof((__eeprom_data*)0)->eeprom_field))

#define eeprom_write(src, eeprom_field) { typeof(src) x = src; eeprom_write_from(&x, eeprom_field, sizeof(x)); }

#define MIN(x,y) ( x > y ? y : x )

static const long EEPROM_MAGIC = 0xcafe;

struct __eeprom_data {
  long magic;
  int  resetCount;
  char mqttClientId[16];                     // ToDo: Should be variable length
  byte mqttServerIp[4];
  int  mqttServerPort;
  byte networkIp[4];
  byte networkGateway[4];
  byte networkMac[4];
  byte networkNetmask[4];
};

byte eepromInitialized = false;

void eepromInitialize(void) {
  long magic;

  if (! eepromInitialized) {
    eeprom_read(magic, magic);
    if (magic != EEPROM_MAGIC) commandSetup();

    // commandPut("resetCount", commandGet("resetCount") + 1);
    int resetCount;
    eeprom_read(resetCount, resetCount);
    resetCount ++;
    eeprom_write(resetCount, resetCount);

    eepromInitialized = true;
  }
}

void commandDelete(void) {
  eventLog("delete()");
}

void commandGet(void) {
  eventLog("get()");

//Serial.println(& message[argumentIndex]);
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

void commandSetup(void) {
  eeprom_write(  0,                resetCount);
  eeprom_write(& MQTT_CLIENT_ID,   mqttClientId);
  eeprom_write(& MQTT_SERVER_IP,   mqttServerIp);
  eeprom_write(  MQTT_SERVER_PORT, mqttServerPort);
  eeprom_write(& NETWORK_IP,       networkIp);
  eeprom_write(& NETWORK_GATEWAY,  networkGateway);
  eeprom_write(& NETWORK_MAC,      networkMac);
  eeprom_write(& NETWORK_NETMASK,  networkNetmask);
  eeprom_write(  EEPROM_MAGIC,     magic);

  eventLog("Setup EEPROM");           // EEPROM serial baud-rate must be setup
  delay(1000);                        // Ensure LCD message can be seen briefly
}

void commandUnlock(void) {
  eventLog("unlock()");
}

void commandVerify(void) {
  eventLog("verify()");
}

int getIntEEPROM(                                 // ToDo: Error return value ?
  char *key) {

  String keyString = String(key);
  int value = -1;

  if (keyString == "resetCount")     eeprom_read(value, resetCount);
  if (keyString == "mqttServerPort") eeprom_read(value, mqttServerPort);

  return(value);
}

void getOctetsEEPROM(                             // ToDo: Error return value ?
  char *key,
  byte  length,
  byte *value1,
  byte  value2[]) {

  String keyString = String(key);

  if (keyString == "mqttServerIp")   eeprom_read(value1, mqttServerIp);
  if (keyString == "networkIp")      eeprom_read(value1, networkIp);
  if (keyString == "networkGateway") eeprom_read(value1, networkGateway);
  if (keyString == "networkMac")     eeprom_read(value1, networkMac);
  if (keyString == "networkNetmask") eeprom_read(value1, networkNetmask);

  for (byte index = 0; index < length; index ++) value2[index] = value1[index];
}


void getStringEEPROM(                             // ToDo: Error return value ?
  char *key,
  char *value1,
  char  value2[]) {

  String keyString = String(key);
  int    length    = 0;

  if (keyString == "mqttClientId") {
    eeprom_read(value1, mqttClientId);
    length = 16;
  }

  for (byte index = 0; index < length; index ++) value2[index] = value1[index];
}

/* ------------------------------------------------------------------------- */
