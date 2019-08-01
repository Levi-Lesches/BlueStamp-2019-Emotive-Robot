/*
  This sketch lists all I2C devices it can find
  It was taken from online, and I do not claim it to be my own
  Helpful for debugging any I2C device

  Specs:
    Program storage: 11%
    Dynamic memory: 26%
*/

#include <Wire.h>

void setup() {
  Wire.begin();
 
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
}
 
void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Serial.println (address);
    Wire.beginTransmission(address);

    // MY ADDITION: 
    // This line could fail if there is an issue with any I2C device
    // The surrounding print statements are to check if this call ever finishes
    Serial.print("Scanning address ");  
    error = Wire.endTransmission();  
    Serial.println("Success");  // My addition
 
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    } else if (error==4) {
      Serial.print("Unknown error at address 0x");
      if (address<16) Serial.print("0");
      Serial.println(address,HEX);
    }
  }

  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
  delay(5000);           // wait 5 seconds for next scan
}
