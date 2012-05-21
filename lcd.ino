/* lcd.ino
 * ~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * ----------------------------------------------------------------------------
 *
 * Description
 * ~~~~~~~~~~~
 * - None, yet.
 *
 * Custom character bit map generator: http://www.quinapalus.com/hd44780udg.html
 *
 * To Do
 * ~~~~~
 * - Get LiquidCrystal pin initialization parameters from EEPROM.
 * - Screen should be refreshed from a buffer.
 * - Screen buffer should be updated via MQTT messages.
 * - Screen buffer should be updated via EEPROM configuration.
 */

#include <limits.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(
  PIN_LCD_RS, PIN_LCD_E, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7
);

#define LCD_BACKLIGHT(state)  digitalWrite(PIN_LCD_BACKLIGHT, state)

byte lcdInitialized = false;

unsigned long lcdClearFieldCounter = LONG_MAX;  // AAG: Use Aiko one-shot ?

static const char bitmapBackslash[] = { 0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00 };

void lcdCreateCustomCharacter(
  byte identifier,
  const char bitMap[]) {

  lcd.command(0x40 + identifier);
  for (byte index = 0;  index < 8;  index ++) lcd.write(bitMap[index]);
  lcd.command(0x80);
}

void lcdInitialize(void) {
  if (! lcdInitialized) {
    pinMode(PIN_LCD_BACKLIGHT, OUTPUT);

    lcd.begin(16, 2);
    lcdCreateCustomCharacter(0, bitmapBackslash);
    lcd.setCursor(0, 0);
    lcd.print("aiko_mqtt_lcd");

    LCD_BACKLIGHT(true);

    lcdInitialized = true;
  }
}

void lcdHandler(void) {
  if (! lcdInitialized) lcdInitialize();

  if (secondCounter > lcdClearFieldCounter) {
    lcd.setCursor(0, 1);
    lcd.print("                ");

    lcdClearFieldCounter = LONG_MAX;
  }
}

byte throbberEnabled = false;
byte throbberIndex = 0;

char throbber[] = "|/-\0|/-\0";     // "\0" is the first custom character

void throbberHandler(void) {
  char throb = (throbberEnabled == true)  ?  throbber[throbberIndex]  :  '?';
  lcd.setCursor(15, 0);
  lcd.print(throb);
  cycleIncrement(throbberIndex, 1, 8);
}

/* ------------------------------------------------------------------------- */
