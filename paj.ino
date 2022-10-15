
#include <Wire.h>

#include "paj7620.h"

void setup() {
  // Initialise serial connection to display results
  Serial.begin(9600);

  // Initialise the sensor
  int error = paj7620Init();
  if(error) {
    Serial.print(F("Initialisation error code: "));
    Serial.println(error);
  }
  else {
    Serial.println(F("Ready!"));
  }
}

void loop() {
  // Create a variable to hold the value of any gesture recognised
  byte gesture;
  // Error variable holds any error code
  int error;
  // Read Reg 0x43 of Bank 0 to get result of any recognised gesture, and store in 'gesture' variable
  error = paj7620ReadReg(0x43, 1, &gesture);
 
  if(!error) {
    switch (gesture) {
      case GES_RIGHT_FLAG:
        Serial.println(F("Derecha"));          
        break;
      case GES_LEFT_FLAG: 
        Serial.println(F("Izquierda"));         
        break;
      case GES_UP_FLAG:
        Serial.println(F("Arriba"));        
        break;
      case GES_DOWN_FLAG:
        Serial.println(F("Abajo"));         
        break;
      case GES_FORWARD_FLAG:
        Serial.println(F("Adelante"));
        break;
      case GES_BACKWARD_FLAG:     
        Serial.println(F("Atras"));
        break;
      
      default:
        break;
    }
  }
 else {
    Serial.print(F("Error code: "));
    Serial.println(error);
 }
  // Introduce small delay before next polling the sensor 
  delay(100);
}
