/* mqtt.ino
 * ~~~~~~~~
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
 * - Debug messages for progress.  // AAG
 * - All delay times (Ethernet and MQTT) should be literal constants. // AAG
 * - Fix 10 second delay on re-connect !
 * - In MQTT callback, invoke command parser.  // AAG
 * - Buttons publish to MQTT topic.  // AAG
 * - LCD subscribes to MQTT topic.
 * - MQTT ClientId should be variable length string.
 * - MQTT topics driven by EEPROM.
 *   - Implement EEPROM code properly first ?
 * - Controller / device health monitoring and reporting.
 * - Abstraction levels ...
 *   - Drive all Arduino I/O pins via MQTT.
 *   - Specific devices: RGB LED, Buttons, LCD, Temperature, Humdity, ...
 *   - Specific actions: RGB LED: Colour, Blinking, Pulsating, etc
 *   - Scheduling and autonomous control (subsumption architecture).
 * - Aiko message protocol ...
 *   - Aiko boot integration (Etherten standard firmware).
 *   - Idempotent messages.
 *   - Measurement calculus.
 *   - Security, e.g xxtea.
 *
 * - http://knolleary.net/2011/05/25/staying-connected
 * - https://gist.github.com/900885
 *   - Uniqueness of all addresses and identifiers per Arduino
 *   - Ethernet controller reset code and re-leasing of addresses ...
 *     in setup() and whenever MQTT disconnection occurs
 *   - Use of “last will and testament” in Arduino client code and Lua scripts
 */

#include <limits.h>

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

void callback(
  char* topic,
  byte* payload,
  int   length) {

  Serial.print("Topic: ");
  Serial.print(topic);

  Serial.print(", Payload: ");
  for (int index = 0;  index < length;  index ++) {
    char ch = payload[index];
    Serial.print(ch);
  }

  Serial.println();
}

PubSubClient mqttClient;

char clientId[4];                              // ToDo: Variable clientId length

byte mqttInitialized = false;

int mqttCounter = 0;

unsigned long mqttDelayCounter = 0;  // AAG: Use Aiko one-shot ?

void mqttInitialize(void) {
  if (! mqttInitialized) {
    serialInitialize();

    delay(500);   // Make cold-start-up more reliable  // AAG: No delay()s

    byte ip[4];
    byte gateway[4];
    byte mac[6];
    byte netmask[4];

    getOctetsEEPROM("networkIp",      4, ip,      ip);
    getOctetsEEPROM("networkGateway", 4, gateway, gateway);
    getOctetsEEPROM("networkMac",     6, mac,     mac);
    getOctetsEEPROM("networkNetmask", 4, netmask, netmask);

    Ethernet.begin(mac, ip, gateway, netmask);

    delay(1000);  // Makes cold-start-up more reliable  // AAG: No delay()s

    getStringEEPROM("mqttClientId", clientId, clientId);

    byte server[4];

    getOctetsEEPROM("mqttServerIp", 4, server, server);

    int port = getIntEEPROM("mqttServerPort");

    mqttClient = PubSubClient(server, port, callback);

    mqttInitialized = true;
mqttCounter = 42;      // AAG: Why isn't this correctly initialized ?
mqttDelayCounter = 0;  // AAG: Why isn't this correctly initialized ?
  }
}

void mqttHandler(void) {
  if (! mqttInitialized) mqttInitialize();

  if (mqttClient.connected() == false  &&  secondCounter > mqttDelayCounter) {
    throbberEnabled = false;

    mqttCounter ++;
    Serial.print("Connecting: ");
    Serial.println(mqttCounter);

    if (mqttClient.connect(clientId)) {        // "arduino/"  + clientId;
      char topic[] = "device/arduino/01";      // MQTT_PREFIX + clientId;  // AAG
      mqttClient.publish(topic, "(connect)");  // "boot" or "connect" ?
      mqttClient.subscribe(topic);
    }
    else {
mqttInitialized = false;  // AAG: May also need to re-initialize Ethernet (if unplugged)
      mqttDelayCounter = secondCounter + 10;
    }
  }
  else {
    throbberEnabled = true;
 
    if (mqttClient.loop()) {
    }
  }
}
