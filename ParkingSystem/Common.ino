int enterCounter = 1;
int exitCounter = 1;
bool passwordGetting = false;
String toExitCarNumber = "";
bool buzzEnabled = false;
int errorCounter = 0;
String split(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String time_now(){
  tmElements_t tm;
  String time_str = "";
  if (RTC.read(tm)) {
    Serial1.print("Ok, Time = ");
    time_str = String(tm.Hour) +":"+ String(tm.Minute) +":"+ String(tm.Second);
    Serial1.println(time_str);  
  } else {
    if (RTC.chipPresent()) {
      Serial1.println("The DS1307 is stopped.  Please run the SetTime");
      Serial1.println("example to initialize the time and begin running.");
      Serial1.println();
    } else {
      Serial1.println("DS1307 read error!  Please check the circuitry.");
      Serial1.println();
    }
  }
  return time_str;
}

String generate_password(){
  randomSeed(random(0, 1000));
  long password = random(100000, 999999);
  return String(password);
}

int cost_calculation(String s_time, String e_time){
  int s_h = split(s_time, ':', 0).toInt();
  int s_m = split(s_time, ':', 1).toInt();
  int e_h = split(e_time, ':', 0).toInt();
  int e_m = split(e_time, ':', 1).toInt();
  int mindiff = 60 + e_m - s_m;
  int hr_diff = ((mindiff/60) - 1) + e_h - s_h;
  return  hr_diff * 1000;
}

int apply_reservation_cost(int cost){
  int r_cost = (0.2 * cost) + cost;
  return r_cost;
}

void manual_inputs(){
  save_car("12 b 222");
  delay(5000);
  save_car("12 b 122");
  delay(5000);
  exit_car("12 b 222", "163264");
  delay(5000);
  save_car("12 b 322");
  delay(5000);
}

void ExitInterrupt(){
  buzzEnabled = false;
  String carNumber = get_car_from_samples(exitCounter);
  Serial1.print("Exit car command: ");
  carNumber.trim();
  Serial1.println(carNumber);
  passwordGetting = true;
  lcd_clear();
  toExitCarNumber = carNumber;
}

void compare_password(String password, int counter){
  if (counter < 6){
    lcd_clear();
    lcd_print("incorrect.");
    errorCounter += 1;
    if (errorCounter == 3){
      passwordGetting = false;
      buzzEnabled = true;
    }
    delay(2000);
  }
  else {
    bool exited = exit_car(toExitCarNumber, password);
    if (exited){
      exitCounter += 1;
      toExitCarNumber = "";
    }
    else {
      errorCounter += 1;
      if (errorCounter == 3){
        passwordGetting = false;
        buzzEnabled = true;
      }
    }
    
  }
}

void EnterInterrupt(){
  buzzEnabled = false;
  String carNumber = get_car_from_samples(enterCounter);
  Serial1.print("Enter car command: ");
  carNumber.trim();
  Serial1.println(carNumber);
  bool saved = save_car(carNumber);
  if (saved){
    enterCounter += 1;  
  }
}

bool password_getting(){
  return passwordGetting;
}

bool buzz_enabled(){
  return buzzEnabled; 
}
