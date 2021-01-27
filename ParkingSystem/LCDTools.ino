#include <LiquidCrystal.h>


const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void lcd_begin(){
  lcd.begin(16, 2);
  lcd.print("Parking System.");
}

void lcd_clear(){
  lcd.clear();
}

void lcd_print(String str){
  lcd.print(str);
}
