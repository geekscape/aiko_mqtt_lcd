/* aiko_mqtt_lcd.h
 * ~~~~~~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * ----------------------------------------------------------------------------
 *
 * To Do
 * ~~~~~
 * - None, yet !
 */

// Allocate strings in Flash program memory to save RAM
// http://arduino.cc/en/Reference/PROGMEM

#include <avr/pgmspace.h>
#define PROGSTRING(name) static const prog_uchar name[] PROGMEM
// PROGSTRING(name) = { "value" };

// MQTT parameters

static const char MQTT_CLIENT_ID[] = "arduino1";
static const byte MQTT_SERVER_IP[] = { 192, 168, 1, 111 };
static const int  MQTT_SERVER_PORT = 1883;

// TCP/IP network parameters for Ethernet

static const byte NETWORK_IP[]      = { 192, 168, 1,  30 };
static const byte NETWORK_GATEWAY[] = { 192, 168, 1,   1 };
static const byte NETWORK_MAC[]     = { 0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed};
static const byte NETWORK_NETMASK[] = { 192, 168, 1, 255 };

// Serial communications speed

static const long DEFAULT_BAUD_RATE = 38400;

/* ------------------------------------------------------------------------- */
// Digital Input/Output pins

static const byte PIN_SERIAL_RX     =  0;  // In:  USB
static const byte PIN_SERIAL_TX     =  1;  // Out: USB

// Standard HD44780 16x2 LCD display
static const byte PIN_LCD_BACKLIGHT =  3;  // Out: LCD Backlight, high is on
static const byte PIN_LCD_D4        =  4;  // Out: LCD data bit 4
static const byte PIN_LCD_D5        =  5;  // Out: LCD data bit 5
static const byte PIN_LCD_D6        =  6;  // Out: LCD data bit 6
static const byte PIN_LCD_D7        =  7;  // Out: LCD data bit 7
static const byte PIN_LCD_RS        =  8;  // Out: LCD RS pin
static const byte PIN_LCD_E         =  9;  // Out: LCD E pin

// Analog Input pins

static const byte PIN_BUTTONS       =  0;  // In:  Button voltage ladder
                                           //      Up/Down/Left/Right/Select

extern byte buttonIndex;                     // See button.ino

#include <LiquidCrystal.h>
extern LiquidCrystal lcd;                    // See event.ino and lcd.ino

extern unsigned long lcdClearFieldCounter;   // See event.ino and lcd.ino

extern char message[];                       // See event.ino and serial.ino
extern byte argumentIndex;                   // Ditto

extern unsigned long secondCounter;          // See aiko_mqtt_lcd.ino

/* ------------------------------------------------------------------------- */
