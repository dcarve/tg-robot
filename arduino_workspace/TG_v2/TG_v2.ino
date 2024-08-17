#include <Arduino.h>
#include <math.h>
#include <util/atomic.h>
#include "pinOutIn.h"
#include "runMotors.h"

void setup() {
  // Declare pins as output:

  blueToothDriverPins();
  allDriversStep();
  stepperDriver1(); // 0°
  stepperDriver2(); // 120° 
  stepperDriver3(); // -120°

}

void loop() {

  stepResolution(ONE_THIRTY_SECOND_STEP)

}


//void setup() {
  // Declare pins as output:
  //pinMode(PB1, OUTPUT); //dir - laranja
  //pinMode(PB0, OUTPUT); //step - roxo
 // pinMode(PA7, OUTPUT); //M0 - preto/laranja
//}

//void loop() {
  // Set the spinning direction clockwise:
  //digitalWrite(PB1, HIGH);
  //digitalWrite(PA7, HIGH);



  //digitalWrite(PB0, HIGH);
  //delayMicroseconds(2000);
  //digitalWrite(PB0, LOW);
  //delayMicroseconds(2000);

  
  //delay(1000);
  //digitalWrite(PB1, LOW);
  //digitalWrite(PB0, LOW);
  //delay(1000);
//}