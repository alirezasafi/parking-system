#include <SPI.h>
#include <SD.h>

int chipSelect = 53;
File smplesFile;
File carsFile;

String samplesFileName = "samples.txt";
String carsFileName = "cars.txt";

const int parkingCapacity = 10;
int occupiedPlace[parkingCapacity];

int initialize_sd_card(){
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) {
      Serial1.println("initialization failed!");
      while (1);
  }
  // free occupied place
  for (int i = 0; i < parkingCapacity ; i++){
    occupiedPlace[0] = 0;
  }
  // clear cars details file.
  remove_file(carsFileName);
  // write header
  write_to_cars_file("Car,Password,Time,Place");
  
//  save_car("12 b 222");
//  save_car("12 b 122");
//  exit_car("12 b 122", "000000");
}

int take_place(){
  for (int i = 0; i < parkingCapacity; i++){
    if (occupiedPlace[i] == 0){
      occupiedPlace[i] = 1;
      return i;
    }
  }
  return -1;
  
}
void save_car(String carNumber){
  if (is_saved_car(carNumber, false) == "1"){
    Serial1.println("error: this car already registered.");
    return;
  }
  int place = take_place();
  
  if (place == -1){
    Serial1.println("error: all the places are occupied.");
    return;
  }
  print_occupied_place();
  Serial1.println("successfully saved.");
  write_to_cars_file(carNumber + "," + getnerate_password() + "," + time_now() +","+ String(place));
  //lcd_print("place: " + String(place));
  //lcd_print("password: 0000");
}

void exit_car(String carNumber, String password){
  String carInfo = is_saved_car(carNumber, true);
  if (carInfo == "-1"){
    Serial1.print(carNumber);
    Serial1.println(" Not found.");
    return;
  }
  if (password.equals(split(carInfo, ',', 1)) == 0){
    Serial1.println("error: password is incorrect.");
    return;
  }
  // get current time
  // cost calculation
  // get place
  int place = split(carInfo, ',', 3).toInt();
  revoke_place(place);
  // print_occupied_place();
  // delete record;
  
}
void print_occupied_place(){
  for (int i = 0; i < parkingCapacity; i++){
    Serial1.print(occupiedPlace[i]);
  }
}
void revoke_place(int place){
  occupiedPlace[place] = 0;
}
String is_saved_car(String carNumber, bool return_record){
  carsFile = SD.open(carsFileName);
  if (carsFile){  
    while(carsFile.available()){
      String carRecord = carsFile.readStringUntil('\n');
      if (carNumber.equals(split(carRecord, ',', 0)) == 1){
        if (return_record == true){
          return carRecord;
        }
        return "1";
      }
    }
  }
  else {
    Serial1.println("error: can't open cars file ...");
  }
  carsFile.close();
  return "-1";
}

void write_to_cars_file(String str){
  carsFile = SD.open(carsFileName, FILE_WRITE);
  if (carsFile){
    Serial1.println("Writing to cars file ...");
    carsFile.println(str);
  }
  else {
    Serial1.println("error: can't open cars file ...");
  }
  carsFile.close();
}

void remove_file(String file_name){
  if (SD.exists(file_name)){
    SD.remove(file_name);
  }
}

void generate_samples(){
  if (!SD.exists(samplesFileName)){
    smplesFile = SD.open(samplesFileName, FILE_WRITE);
    if (smplesFile) {
      Serial1.println("Writing sample data to samples.txt...");
      char *nums[] = {"12 b 810", "77 x 120", "10 k 901", "48 k 710"};
      for (int i = 0; i<4; i++){
        smplesFile.println(nums[i]);    
      }
     
      smplesFile.close();
      Serial1.println("done.");
    } else {
      Serial1.println("error opening samples.txt");
    }
  }
}
