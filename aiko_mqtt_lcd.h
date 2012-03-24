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

// Serial communications speed

static const long DEFAULT_BAUD_RATE = 38400;

/* ------------------------------------------------------------------------- */
// Digital Input/Output pins

static const int PIN_SERIAL_RX      =  0;  // In:  USB
static const int PIN_SERIAL_TX      =  1;  // Out: USB

// Standard HD44780 16x2 LCD display
static const int PIN_LCD_BACKLIGHT  =  3;  // Out: LCD Backlight, high is on
static const int PIN_LCD_D4         =  4;  // Out: LCD data bit 4
static const int PIN_LCD_D5         =  5;  // Out: LCD data bit 5
static const int PIN_LCD_D6         =  6;  // Out: LCD data bit 6
static const int PIN_LCD_D7         =  7;  // Out: LCD data bit 7
static const int PIN_LCD_RS         =  8;  // Out: LCD RS pin
static const int PIN_LCD_E          =  9;  // Out: LCD E pin

// Analog Input pins

static const int PIN_BUTTONS        =  0;  // In:  Button voltage ladder
                                           //      Up/Down/Left/Right/Select

extern byte buttonIndex;                     // See button.ino

#include <LiquidCrystal.h>
extern LiquidCrystal lcd;                    // See event.ino and lcd.ino

extern void commandClear(void);              // See eeprom.ino
extern void commandDelete(void);
extern void commandGet(void);
extern void commandLock(void);
extern void commandPut(void);
extern void commandReset(void);
extern void commandUnlock(void);
extern void commandVerify(void);

extern void eventLog(void);                  // See event.ino

extern unsigned long lcdClearFieldCounter;   // See event.ino and lcd.ino

extern unsigned long secondCounter;          // See aiko_mqtt_lcd.ino

/* ------------------------------------------------------------------------- */
