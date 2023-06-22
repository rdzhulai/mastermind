#include "lcd_wrapper.h"

LiquidCrystal_I2C lcd(0x27, 12, 2);

void lcd_init()
{
    lcd.init();
    lcd.backlight();
}