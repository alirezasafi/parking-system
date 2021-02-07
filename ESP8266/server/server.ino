#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

StaticJsonDocument<200> JSON_Packet;
String inputString = "";
ESP8266WebServer server(80);

const int led = 13;
void cors_config(){
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Allow", "HEAD,GET,PUT,POST,DELETE,OPTIONS");
  server.sendHeader("Access-Control-Allow-Methods", "GET, HEAD, POST, PUT");
  server.sendHeader("Access-Control-Allow-Headers", "X-Requested-With, X-HTTP-Method-Override, Content-Type, Cache-Control, Accept");
}
void handleRoot() {
  server.send(200, "application/json", make_message_data("Welcome to the parking system."));
}

void handleNotFound() {
  if (server.method() == HTTP_OPTIONS){
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.sendHeader("Access-Control-Max-Age", "10000");
        server.sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
        server.sendHeader("Access-Control-Allow-Headers", "*");
        server.send(204);
  }
  else {
    server.send(404, "application/json", make_message_data("Not Found."));  
  }
}

void handleReserve(){
  if (server.method() == HTTP_OPTIONS){
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.sendHeader("Access-Control-Max-Age", "10000");
        server.sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
        server.sendHeader("Access-Control-Allow-Headers", "*");
        server.send(204);
  }
  else {
    JSON_Packet.clear();
    inputString = "";
    for (uint8_t i = 0; i < server.args(); i++) {
      inputString += server.arg(i);
    }
    int body_err = deserialize_data();
    if (body_err == -1){
      cors_config();
      server.send(400, "application/json", car_id_required());  
    }
    else {
      serializeJson(JSON_Packet, Serial);
      Serial.println();
      delay(1000);
      get_serial_response();
      JSON_Packet.clear();
      int code = deserialize_data();
      cors_config();    
      if (code == 201){
        server.send(code, "application/json", json_to_string());
      }
      else if(code == -1){
       server.send(500, "application/json", make_message_data("internal server error.")); 
      }
      else {
        server.send(code, "application/json", make_message_data("Bad reuqest"));
      }
    }   
  }
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/reserve/", handleReserve);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
String make_message_data(String message){
  JSON_Packet.clear();
  JSON_Packet["message"] = message;
  return json_to_string();
}

String car_id_required(){
  JSON_Packet.clear();
  JSON_Packet["car_id"] = "This field is required.";
  return json_to_string();
}
String json_to_string(){
  String str_data;
  serializeJson(JSON_Packet, str_data);
  JSON_Packet.clear();
  return str_data;
  
}
void get_serial_response(){
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    }
}
int deserialize_data(){
  JSON_Packet.clear();
  DeserializationError error = deserializeJson(JSON_Packet, inputString);
  inputString = "";
  
  if (!error) {
    if(JSON_Packet.containsKey("code")){
      int code = JSON_Packet["code"];
      inputString = "";
      return code;
    } 
    if(JSON_Packet.containsKey("car_id")){
      inputString = "";
      return 1; 
    }
    if(JSON_Packet.containsKey("place")){
      inputString = "";
      return 201;
    }
 }
 JSON_Packet.clear();
 inputString = "";
 return -1;
}
