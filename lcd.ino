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
 * To Do
 * ~~~~~
 * - Get LiquidCrystal pin initialization parameters from EEPROM.
 */

#include <limits.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(
  PIN_LCD_RS, PIN_LCD_E, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7
);

#define LCD_BACKLIGHT(state)  digitalWrite(PIN_LCD_BACKLIGHT, state)

byte lcdInitialized = false;

unsigned long lcdClearFieldCounter = LONG_MAX;

void lcdInitialize(void) {
  if (! lcdInitialized) {
    pinMode(PIN_LCD_BACKLIGHT, OUTPUT);

    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("aiko_mqtt_lcd v0");

    LCD_BACKLIGHT(true);

    lcdInitialized = true;
  }
}

void lcdHandler(void) {
  if (! lcdInitialized) lcdInitialize();

  if (secondCounter > lcdClearFieldCounter) {
    lcd.setCursor(0,1);
    lcd.print("                ");

    lcdClearFieldCounter = LONG_MAX;
  }
}

/* ------------------------------------------------------------------------- */
