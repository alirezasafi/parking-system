#include <Keypad.h>
#include <stdbool.h>


const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {22, 23, 24, 25}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {26, 27, 28}; //connect to the column pinouts of the keypad

String inputPass = "";
int passwordAttempt = 0;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
  
char keypad_get_key(){
  char key = keypad.getKey();
  return key;
}

int checkPassword(String password) {
  
  char customKey = keypad_get_key();
  if(customKey && customKey != '*' && customKey != '#'){
    inputPass += customKey;
    Serial1.print(customKey);
  }
  else if (customKey == '#'){
    inputPass = "";
    Serial1.println("  Canceled. Type again ...");
    lcd_clear();
    lcd_print("Canceled.");
  
  }
  else if(customKey == '*'){
    if(inputPass.compareTo(password) == 0){
      lcd_clear();
      lcd_print("correct.");
      Serial1.println(" ===> password is correct."); 
      inputPass = "";
      passwordAttempt = 0;
      digitalWrite(buzz, LOW);
      return 1;
    }
    passwordAttempt++;
    if(passwordAttempt == 1){
      digitalWrite(buzz, HIGH);
    }
    inputPass = "";
    lcd_clear();
    lcd_print("incorrect.");
    Serial1.println(" ===> password is incorrect."); 
    return 0;
  }
}
