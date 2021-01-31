#include <SPI.h>
#include <SD.h>

int chipSelect = 53;
File samplesFile;
File carsFile;
File carsFileTemp;

String samplesFileName = "samples.txt";
String carsFileName = "cars.txt";
String carsFileTempName = "temp.txt";

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
  // check the car was registered or not.
  if (is_saved_car(carNumber, false) == "1"){
    Serial1.println("error: this car already registered.");
    return;
  }

  // find a place. if capacity is full print error.
  int place = take_place();
  if (place == -1){
    Serial1.println("error: all the places are occupied.");
    return;
  }
  
  // save car record in text file.
  Serial1.println("successfully saved.");
  String password = generate_password();
  write_to_cars_file(carNumber + "," + password + "," + time_now() +","+ String(place));

  // print place and password in the lcd.
  lcd_clear();
  lcd_print("place: " + String(place));
  lcd_set_cursor(0,1);
  lcd_print("password: "+ password);
}

void exit_car(String carNumber, String password){
  lcd_clear();
  String carInfo = is_saved_car(carNumber, true);

  // raise exception if car not found.
  if (carInfo == "-1"){
    Serial1.print(carNumber);
    Serial1.println(" Not found.");
    return;
  }
  
  // authenticate password.
  if (password.equals(split(carInfo, ',', 1)) == 0){
    Serial1.println("error: password is incorrect.");
    lcd_print("password is incorrect.");
    return;
  }
  
  // cost calculation and print in the lcd.
  String s_time = split(carInfo, ',', 2);
  String e_time = time_now();
  int cost = cost_calculation(s_time, e_time);

  // revoke the occupied place 
  int place = split(carInfo, ',', 3).toInt();
  revoke_place(place);

  // delete car record.
  bool removed = delete_car(carNumber);
  if (removed == true){
    Serial1.println("car successfully removed.");
  }
  else {
    Serial1.println("failed to remove.");
  }
}

bool delete_car(String carNumber){
  String carBuffer;
  bool removed = false;
  carsFile = SD.open(carsFileName);
  carsFileTemp = SD.open(carsFileTempName, FILE_WRITE);
  if (carsFileTemp){
    if (carsFile){
      while(carsFile.available()){
        carBuffer = carsFile.readStringUntil('\n');
        if (carNumber.equals(split(carBuffer, ',', 0)) != 1){
          carsFileTemp.println(carBuffer.substring(0, carBuffer.length() - 1));
        }
        else {
          removed = true;
        }
      }
      carsFile.close();
      remove_file(carsFileName);
      carsFileTemp.close();
      copy_and_delete_temp_cars_file();
      
    }
    else {
      Serial1.println("delet_car_error: can't open cars file ...");
    }
  }
  else {
    Serial1.println("delet_car_error: can't open temp cars file ...");
  }
  return removed;
}
void copy_and_delete_temp_cars_file(){
  carsFile = SD.open(carsFileName, FILE_WRITE);
  carsFileTemp = SD.open(carsFileTempName);
  if (carsFile){
    if (carsFileTemp){
      while(carsFileTemp.available()){
        carsFile.write(carsFileTemp.read()); 
      }
      carsFile.close();
      carsFileTemp.close();
      remove_file(carsFileTempName);
    }
    else {
      Serial1.println("copy_error: can't open temp cars file ...");
    }
  }
  else {
    Serial1.println("copy_error: can't open cars file ...");
  }
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
  // check the car was registered or not 
  // return type: '1': registered car, '-1': unregistered car.
  // if return_record is true: return car record.
                
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
    carsFile.close();
  }
  else {
    Serial1.println("error: can't open cars file ...");
  }
  return "-1";
}

void write_to_cars_file(String str){
  // write a string in the cars.txt.
  
  carsFile = SD.open(carsFileName, FILE_WRITE);
  if (carsFile){
    //Serial1.println("Writing to cars file ...");
    carsFile.println(str);
    carsFile.close();
  }
  else {
    Serial1.println("error: can't open cars file ...");
  }
}

void remove_file(String file_name){
  if (SD.exists(file_name)){
    SD.remove(file_name);
  }
}

void generate_samples(){
  if (!SD.exists(samplesFileName)){
    samplesFile = SD.open(samplesFileName, FILE_WRITE);
    if (samplesFile) {
      Serial1.println("Writing sample data to samples.txt...");
      char *nums[] = {"12 b 810", "77 x 120", "10 k 901", "48 k 710"};
      for (int i = 0; i<4; i++){
        samplesFile.println(nums[i]);    
      }
     
      samplesFile.close();
      Serial1.println("done.");
    } else {
      Serial1.println("error opening samples.txt");
    }
  }
}
