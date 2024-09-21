import RPi.GPIO as GPIO


import pin_out_in
#from encoders import *
#from run_motors import *
#from serial import *

import time


GPIO.setmode(GPIO.BCM)

GPIO = pin_out_in.setup_pin_driver1(GPIO)  #0º
GPIO = pin_out_in.setup_pin_driver2(GPIO)  #120º
GPIO = pin_out_in.setup_pin_driver3(GPIO)  #-120º
GPIO = pin_out_in.setup_pin_step_resulotion(GPIO)

#include <AccelStepper.h>

DT_TIME_SEND_MOTOR_DATA = 0.100
DT_BLUETOOTH_DATA = 0.3
DT_CALC_SPEED = 300
MAX_ACCELERATION = 1000
MAX_SPEED = 50000

w1 = 0
w2 = 0
w3 = 0

direction_angle = 90.0
linear_speed_percent = 0.0
angular_speed = 0.0

nextMotorData  = (time.time() + DT_TIME_SEND_MOTOR_DATA)
nextBtData  = (time.time() + DT_BLUETOOTH_DATA)
nextCalcSpeed  = (time.time() + DT_CALC_SPEED)

bufferSize = 100
inputBuffer=list()
bufferIndex = 0

#AccelStepper stepper1(AccelStepper::DRIVER, STEP_1, DIR_1);
#AccelStepper stepper2(AccelStepper::DRIVER, STEP_2, DIR_2);
#AccelStepper stepper3(AccelStepper::DRIVER, STEP_3, DIR_3);


  #Declare pins as output:

  #stepResolution(ONE_THIRTY_SECOND_STEP);
  #setUpSerialMonitor();
  #setUpSerialUsart3();

  #stepper1.setMaxSpeed(MAX_SPEED);  // Max speed (steps per second)
  #stepper1.setAcceleration(MAX_ACCELERATION);  // Acceleration (steps per second^2)

  #stepper2.setMaxSpeed(MAX_SPEED);
  #stepper2.setAcceleration(MAX_ACCELERATION);

  #stepper3.setMaxSpeed(MAX_SPEED);
  #stepper3.setAcceleration(MAX_ACCELERATION);



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

