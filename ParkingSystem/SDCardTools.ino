#include <SPI.h>
#include <SD.h>

int chipSelect = 53;
File samplesFile;
File carsFile;
File carsFileTemp;
File reservedCars;

String samplesFileName = "samples.txt";
String carsFileName = "cars.txt";
String reservedCarsFileName = "reserved.txt";
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
  remove_file(reservedCarsFileName);
  // write header
  write_to_cars_file("Car,Password,Time,Place");
  write_to_reserved_file("Car,Place");
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
int reserve_car(String carNumber){
  int place = is_reserved_car(carNumber);
  if (place == -1){
    place = take_place();
    if (place == -1){
      Serial1.println("error: all the places are occupied.");
      return -1;
    }
  }
  else {
    return -1;
  }
  // save car record in text file.
  Serial1.println("successfully reserved.");
  write_to_reserved_file(carNumber + "," + String(place));
  return place;
}
bool save_car(String carNumber){
  // check the car was registered or not.
  if (is_saved_car(carNumber, false) == "1"){
    Serial1.println("error: this car already registered.");
    return false;
  }
  // find a place. if capacity is full print error.
  int place = is_reserved_car(carNumber);
  if (place == -1){
    place = take_place();
    if (place == -1){
      Serial1.println("error: all the places are occupied.");
      return false;
    }
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
  return true;
}

bool exit_car(String carNumber, String password){
  lcd_clear();
  String carInfo = is_saved_car(carNumber, true);

  // raise exception if car not found.
  if (carInfo == "-1"){
    Serial1.print(carNumber);
    Serial1.println(" Not found.");
    return false;
  }
  
  // authenticate password.
  if (password.equals(split(carInfo, ',', 1)) == 0){
    Serial1.println("error: password is incorrect.");
    lcd_print("incorrect.");
    delay(2000);
    return false;
  }

  // cost calculation and print in the lcd.
  String s_time = split(carInfo, ',', 2);
  String e_time = time_now();
  int cost = cost_calculation(s_time, e_time);
  
  if (is_reserved_car(carNumber) != -1){
    cost = apply_reservation_cost(cost);
  }
  
  lcd_print("correct.");
  lcd_set_cursor(0,1);
  lcd_print("cost: " + String(cost));
  
  // revoke the occupied place 
  int place = split(carInfo, ',', 3).toInt();
  revoke_place(place);

  // delete car record.
  bool removed = delete_car(carNumber);
  if (removed == true){
    Serial1.println("car successfully removed.");
    return true;
  }
  else {
    Serial1.println("failed to remove.");
    return false;
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

int is_reserved_car(String carNumber){
  reservedCars = SD.open(reservedCarsFileName);
  if (reservedCars){
    while(reservedCars.available()){
      String carRecord = reservedCars.readStringUntil('\n');
      if (carNumber.equals(split(carRecord, ',', 0)) == 1){
        return split(carRecord, ',', 1).toInt();
      }
    }
    reservedCars.close();
  }
  else {
    Serial1.println("error: can't open reserved file ...");
  }
  return -1;
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
void write_to_reserved_file(String str){
  // write to reserved file
  
  reservedCars = SD.open(reservedCarsFileName, FILE_WRITE);
  if (reservedCars){
    //Serial1.println("Writing to reserved file ...");
    reservedCars.println(str);
    reservedCars.close();
  }
  else {
    Serial1.println("error: can't open reserved file ...");
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
      char *nums[] = {"12 ب 810", "77 س 120", "10 ص 901", "48 ق 710", "50 د 202"};
      for (int i = 0; i<5; i++){
        samplesFile.println(nums[i]);    
      }
     
      samplesFile.close();
      Serial1.println("done.");
    } else {
      Serial1.println("error opening samples.txt");
    }
  }
}

String get_car_from_samples(int n){
  int counter = 1;
  samplesFile = SD.open(samplesFileName);
  if (samplesFile){
    while(samplesFile.available()){
      String carnumber = samplesFile.readStringUntil('\n');
      if (counter == n){
        return carnumber; 
      }
      counter += 1;
    }
    samplesFile.close();
  }
  else {
    Serial1.println("error: can't open samples file ...");
  }
  return "";
}
