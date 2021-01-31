#include <stdbool.h>
#define buzz 29

void setup() {
  // Open serial communications and wait for port to open:
  Serial1.begin(9600);
  
  pinMode(buzz, OUTPUT);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial1.println("Initializing LCD ...");
  delay(1000);
  
  Serial1.println("Initializing SD card...");
  initialize_sd_card();
  //generate_samples();
  delay(1000);
  
  Serial1.println("initialization done.");
 
}

void loop() {
	
	//checkPassword("1234");
}
