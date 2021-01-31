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
  return "12:00:00";
}

String generate_password(){
  randomSeed(random(0, 1000));
  long password = random(100000, 999999);
  return String(password);
}

int cost_calculation(String s_time, String e_time){
  return 0;
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
