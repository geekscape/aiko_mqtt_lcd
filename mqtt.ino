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
 * - In MQTT callback, invoke command parser.
 * - http://knolleary.net/2011/05/25/staying-connected
 * - https://gist.github.com/900885
 *   - Uniqueness of all addresses and identifiers per Arduino
 *   - Ethernet controller reset code and re-leasing of addresses ...
 *     in setup() and whenever MQTT disconnection occurs
 *   - Use of “last will and testament” in Arduino client code and Lua scripts
 */

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

void callback(
  char* topic,
  byte* payload,
  int   length) {

  Serial.print("Received: ");
  for (int index = 0;  index < length;  index ++) {
    char ch = payload[index];
    Serial.print(ch);
  }
  Serial.println();
}

PubSubClient mqttClient;

char clientId[16];                            // ToDo: Variable clientId length

byte mqttInitialized = false;

void mqttInitialize(void) {
  if (! mqttInitialized) {
    serialInitialize();

    delay(500);   // Make cold-start-up more reliable

    byte ip[4];
    byte gateway[4];
		byte mac[6];
    byte netmask[4];

    getOctetsEEPROM("networkIp",      4, ip,      ip);
    getOctetsEEPROM("networkGateway", 4, gateway, gateway);
    getOctetsEEPROM("networkMac",     6, mac,     mac);
    getOctetsEEPROM("networkNetmask", 4, netmask, netmask);

    Ethernet.begin(mac, ip, gateway, netmask);

    delay(1000);  // Makes cold-start-up more reliable

    char mqttClientId[16];
    getStringEEPROM("mqttClientId", clientId, clientId);

    byte server[4];

    getOctetsEEPROM("mqttServerIp", 4, server, server);

    int port = getIntEEPROM("mqttServerPort");

    mqttClient = PubSubClient(server, port, callback);

    mqttInitialized = true;
  }
}

int count = 0;

void mqttHandler(void) {
  if (! mqttInitialized) mqttInitialize();

  if (mqttClient.connected() == false) {
    count ++;
    Serial.print("Connecting: ");
    Serial.println(count);

    if (mqttClient.connect(clientId)) {
      mqttClient.publish("test/1", "aiko_mqtt_lcd: Start");
      mqttClient.subscribe("test/1");
    }
    else {
      delay(10000);                  // ToDo: Don't stall event processing loop
    }
  }
  else {
    if (mqttClient.loop()) {
    }
  }
}
