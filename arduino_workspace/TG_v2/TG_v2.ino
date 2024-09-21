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

#define DT_TIME_SEND_MOTOR_DATA 100
#define DT_BLUETOOTH_DATA 3
#define DT_CALC_SPEED 300
#define MAX_ACCELERATION 1000
#define MAX_SPEED 50000

volatile long  w1 = 0;  
volatile long  w2 = 0;
volatile long  w3 = 0;

float direction_angle = 90.0;
float linear_speed_percent = 0.0;
float angular_speed = 0.0;

int nextMotorData  = (millis() + DT_TIME_SEND_MOTOR_DATA);
int nextBtData  = (millis() + DT_BLUETOOTH_DATA);
int nextCalcSpeed  = (millis() + DT_CALC_SPEED);

const int bufferSize = 100; // Define the buffer size
char inputBuffer[bufferSize]; // Buffer to store the input string
int bufferIndex = 0; // Index for the buffer

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

  stepResolution(ONE_THIRTY_SECOND_STEP);
  setUpSerialMonitor();
  setUpSerialUsart3();

  stepper1.setMaxSpeed(MAX_SPEED);  // Max speed (steps per second)
  stepper1.setAcceleration(MAX_ACCELERATION);  // Acceleration (steps per second^2)

  stepper2.setMaxSpeed(MAX_SPEED);
  stepper2.setAcceleration(MAX_ACCELERATION);

  stepper3.setMaxSpeed(MAX_SPEED);
  stepper3.setAcceleration(MAX_ACCELERATION);

}

void readUsartConvertRgbToAngleAndMagnitude(){

  while (Serial3.available()) {
    char incomingChar = Serial3.read();

    if (incomingChar == '\n') {
      inputBuffer[bufferIndex] = '\0'; // Null-terminate the string
      rgbToDiretionAngleAndMagnitude(inputBuffer, &direction_angle, &linear_speed_percent);

      Serial.print("direction_angle: ");
      Serial.print(direction_angle, 4);
      Serial.print(" , linear_speed_percent: ");
      Serial.println(linear_speed_percent, 4);

        
      Serial.print("step per seconds 1: ");
      Serial.print(w1);
      Serial.print("    , step per seconds 2: ");
      Serial.print(w2);
      Serial.print("    , step per seconds 3: ");
      Serial.println(w3);
      
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

  SetMotorReset(1, HIGH);
  SetMotorReset(2, HIGH);
  SetMotorReset(3, HIGH);
 
  if (w1==0){SetMotorSleep(1, LOW);} else {SetMotorSleep(1, HIGH);}
  if (w2==0){SetMotorSleep(2, LOW);} else {SetMotorSleep(2, HIGH);}
  if (w3==0){SetMotorSleep(3, LOW);} else {SetMotorSleep(3, HIGH);}


  if (millis()>=nextBtData){
    readUsartConvertRgbToAngleAndMagnitude();  
    nextBtData = millis() + DT_BLUETOOTH_DATA;
  }

  if (millis()>=nextCalcSpeed){

    // TransformationMatrixRpm(
    //   &w1,
    //   &w2,
    //   &w3,
    //   linear_speed_percent,
    //   direction_angle,
    //   angular_speed
    // );


    w1 = (int) (linear_speed_percent * 25600);
    w2 = (int) (linear_speed_percent * 25600);
    w3 = (int) (linear_speed_percent * 25600);

    nextCalcSpeed = millis() + DT_CALC_SPEED;
  }


  if (millis()>=nextMotorData){
    stepper1.setSpeed(w1);
    stepper2.setSpeed(w2);
    stepper3.setSpeed(w3);
    nextMotorData = millis() + DT_TIME_SEND_MOTOR_DATA;
  }

  stepper1.runSpeed();
  stepper2.runSpeed();
  stepper3.runSpeed();


}

