void setup() {
  // Open serial communications and wait for port to open:
  Serial1.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial1.println("Initializing LCD ...");
  delay(1000);
  
  Serial1.println("Initializing SD card...");
  delay(1000);
  
  Serial1.println("initialization done.");
 
}

void loop() {
}
