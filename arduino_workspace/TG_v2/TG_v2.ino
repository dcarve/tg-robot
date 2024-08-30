#include <Arduino.h>
#include <math.h>
#include <util/atomic.h>
#include "pinOutIn.h"
#include "runMotors.h"
#include "encoders.h"
#include "serial.h"
#include <HardwareSerial.h>
#include <libmaple/usart.h>

#include <AccelStepper.h>


#define DT_TIME_SEND_MOTOR_DATA 1000
#define DT_BLUETOOTH_DATA 3
#define MAX_ACCELERATION 200
#define MAX_SPEED 1000

float w1 = 0;  
float w2 = 0;
float w3 = 0;
int w1_delay=0;
int w2_delay=0;
int w3_delay=0;

float direction_angle = 90.0;
float linear_speed_percent = 0.0;
float angular_speed = 0.0;

int nextMotorData  = (millis() + DT_TIME_SEND_MOTOR_DATA);
int nextBtData  = (millis() + DT_BLUETOOTH_DATA);

const int bufferSize = 100; // Define the buffer size
char inputBuffer[bufferSize]; // Buffer to store the input string
int bufferIndex = 0; // Index for the buffer

volatile long motorSpeed = 100;

AccelStepper stepper1(AccelStepper::DRIVER, STEP_1, DIR_1);
AccelStepper stepper2(AccelStepper::DRIVER, STEP_2, DIR_2);
AccelStepper stepper3(AccelStepper::DRIVER, STEP_3, DIR_3);

void setup() {
  // Declare pins as output:

  blueToothDriverPins();
  allDriversStep();
  stepperDriver1(); // 0°
  stepperDriver2(); // 120° 
  stepperDriver3(); // -120°

  stepResolution(HALF_STEP);
  setUpSerialMonitor();
  setUpSerialUsart3();

  stepper1.setMaxSpeed(MAX_SPEED);  // Max speed (steps per second)
  stepper1.setAcceleration(MAX_ACCELERATION);  // Acceleration (steps per second^2)

  stepper2.setMaxSpeed(MAX_SPEED);
  stepper2.setAcceleration(MAX_ACCELERATION);

  stepper3.setMaxSpeed(MAX_SPEED);
  stepper3.setAcceleration(MAX_ACCELERATION);

  stepper1.setSpeed(motorSpeed);
  stepper2.setSpeed(motorSpeed);
  stepper3.setSpeed(motorSpeed);

}

void readUsartConvertRgbToAngleAndMagnitude(){

  while (Serial3.available()) {
    char incomingChar = Serial3.read();

    if (incomingChar == '\n') {
      inputBuffer[bufferIndex] = '\0'; // Null-terminate the string
      rgbToDiretionAngleAndMagnitude(inputBuffer, &direction_angle, &linear_speed_percent);

      Serial.print("direction_angle: ");
      Serial.print(direction_angle);
      Serial.print(" , linear_speed_percent: ");
      Serial.println(linear_speed_percent);
      
      bufferIndex = 0;
    } else {
      if (bufferIndex < bufferSize - 1) {
        inputBuffer[bufferIndex] = incomingChar;
        bufferIndex++;
      } else {
        bufferIndex = 0; // Reset the buffer index
      }
    }
  }
}


void loop() {

  if (millis()>=nextBtData){
    readUsartConvertRgbToAngleAndMagnitude();
    nextBtData = millis() + DT_BLUETOOTH_DATA;
  }


  if (millis()>=nextMotorData){
    stepper1.setSpeed(motorSpeed);
    stepper2.setSpeed(motorSpeed);
    stepper3.setSpeed(motorSpeed);
    nextMotorData = millis() + DT_TIME_SEND_MOTOR_DATA;
  }

  stepper1.runSpeed();
  stepper2.runSpeed();
  stepper3.runSpeed();

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


///código que funciona com lendo blueTooth  LUCAS

// void loop() {
//     while (Serial3.available()) 
//   {
//     delay(3);  
//     c = Serial3.read();
//     readString += c; 
//   }// end while
//   if (readString.length() >0) 
//   {  
//     Serial.println(readString);  
//    readString="";  
//   } // end if
// }




    // TransformationMatrix(
    //   &w1,
    //   &w2,
    //   &w3,
    //   direction_angle,
    //   angular_speed
    // );

    // convertRpmToDelays(
    //   &w1_delay,
    //   &w2_delay,
    //   &w3_delay,
    //   w1,
    //   w2,
    //   w3,
    //   HALF_STEP
    // );

    //SetSleep(1, LOW);
    //SetReset(1, LOW);
    //SetStep(1, LOW);
    //SetStep(1, LOW);
    //SetStep(1, LOW);