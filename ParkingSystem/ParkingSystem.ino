#include <stdbool.h>
#include <ArduinoJson.h>
#define buzz A1

StaticJsonDocument<200> JSON_Packet;
String inputString = "";
bool stringComplete = false; 

String data;
void setup() {
  // Open serial communications and wait for port to open:
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(buzz, OUTPUT);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  inputString.reserve(200);

  // enter and exit interupt.
  attachInterrupt(digitalPinToInterrupt(2), EnterInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(3), ExitInterrupt, RISING);
  
  Serial1.println("Initializing LCD ...");
  lcd_begin();
  delay(2000);
  lcd_clear();
  
  Serial1.println("Initializing SD card...");
  initialize_sd_card();
  generate_samples();
  delay(1000);

  Serial1.println("initialization done.");
  
  //manual_inputs();
  
}

void loop() {
  if (buzz_enabled()){
    buzzing();
  }
  if (password_getting()){
    char key = keypad_get_key();
    Serial1.print(key);
    keypad_controller(key);
  }
  
  if(stringComplete) {
    DeserializationError error = deserializeJson(JSON_Packet, inputString);
    inputString = "";
    stringComplete = false;
    
    if (!error) {
      if(JSON_Packet.containsKey("car_id")){
        const char* car_id = JSON_Packet["car_id"];
        int place = reserve_car(car_id);
        if (place != -1){
          JSON_Packet.clear();
          JSON_Packet["place"] = place;
          send_response(201);
        }
        else {
          send_response(400);
        }
      }
      JSON_Packet.clear();
      inputString = "";
      stringComplete = false;
    }
  }
}

void send_response(int code){
  JSON_Packet["code"] = code;
  serializeJson(JSON_Packet, Serial);
  Serial.println();
  JSON_Packet.clear();
  delay(10);  
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    Serial1.print(inChar);
    if(inChar!=0x0A) 
      inputString += inChar;
    if (inChar == 0x0D) {
      stringComplete = true;
    }
  }
}
void buzzing(){
  digitalWrite(buzz, HIGH);
  delay(500);  
  digitalWrite(buzz, LOW);
  delay(500); 
}
