#include <SPI.h>
#include <SD.h>

File myFile;
int chipSelect = 53;
File OutFile;

String carNumberFileName = "car_nums.txt";
String resultFileName = "result.txt";


int begin_sd_card(){
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) {
      Serial1.println("initialization failed!");
      while (1);
  }
  if (SD.exists(resultFileName)){
    SD.remove(resultFileName);
  }
  OutFile = SD.open(resultFileName, FILE_WRITE);
  String buffer;
  String carNum;
  int pass;
  myFile = SD.open(carNumberFileName);
  if (myFile && OutFile) {
    while (myFile.available()) {
      pass = generatePass();
      carNum = myFile.readStringUntil('\n');
      buffer = "pass: " + String(pass) + ", car num: " + carNum ;
      OutFile.println(buffer);
      lcd.println(carNum);
      lcd.setCursor(0,1);
      lcd.print("pass: "+ String(pass));
      Serial.println(buffer);
      delay(5000);
      lcd.clear();
      
    }
    // close the files:
    myFile.close();
    OutFile.close();
    } else {
    // if the file didn't open, print an error:
    Serial1.println("error opening test2.txt");
    }
}

void generateCarNum(){
  if (!SD.exists(carNumberFileName)){
    myFile = SD.open(carNumberFileName, FILE_WRITE);
    if (myFile ) {
      Serial1.println("Writing sample data to car_nums.txt...");
      char *nums[] = {"365 ب 12", "725 ط 75", "417 د 34", "991 ن 9"};
      for (int i = 0; i<4; i++){
        myFile.println(nums[i]);    
      }
     
      myFile.close();
      Serial1.println("done.");
    } else {
      Serial1.println("error opening car_nums.txt");
    }
  }
}
