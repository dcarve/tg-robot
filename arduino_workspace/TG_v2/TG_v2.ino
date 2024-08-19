#include <Arduino.h>
#include <math.h>
#include <util/atomic.h>
#include "pinOutIn.h"
#include "runMotors.h"
#include "encoders.h"
#include "serial.h"


#define DT_TIME_SEND_MOTOR_DATA 10

float w1 = 0;  
float w2 = 0;
float w3 = 0;
int w1_delay=0;
int w2_delay=0;
int w3_delay=0;
float direction_angle = 90;
float angular_speed = 0;

int nextMotorData  = (millis() + DT_TIME_SEND_MOTOR_DATA);

void setup() {
  // Declare pins as output:

  blueToothDriverPins();
  allDriversStep();
  stepperDriver1(); // 0°
  stepperDriver2(); // 120° 
  stepperDriver3(); // -120°

  stepResolution(HALF_STEP);
  setUpSerialMonitor();

}

void loop() {


  if (millis()>=nextMotorData){

    TransformationMatrix(
      &w1,
      &w2,
      &w3,
      direction_angle,
      angular_speed
    );

    convertRpmToDelays(
      &w1_delay,
      &w2_delay,
      &w3_delay,
      w1,
      w2,
      w3,
      HALF_STEP
    );

    SetSleep(1, LOW);
    SetReset(1, LOW);
    SetStep(1, LOW);
    SetStep(1, LOW);
    SetStep(1, LOW);

    nextMotorData = millis() + DT_TIME_SEND_MOTOR_DATA;
    

  }

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