#include <Arduino.h>
#include <math.h>
#include <util/atomic.h>
#include <AccelStepper.h>
#include "encoders.h"
#include "pinOutIn.h"



#define DT_TIME_SEND_MOTOR_DATA 1000
#define DT_BLUETOOTH_DATA 3
#define DT_CALC_SPEED 500
#define MAX_ACCELERATION 1000
#define MAX_SPEED 1000

AccelStepper stepper2(AccelStepper::DRIVER, PA8, PB15);
AccelStepper stepper3(AccelStepper::DRIVER, PB6, PB5);
//AccelStepper stepper3(AccelStepper::DRIVER, PC14, PC13);

void setup() {
  //Declare pins as output:
  pinMode(PB7, OUTPUT); //sleep
  pinMode(PB8, OUTPUT); //reset
  pinMode(PB6, OUTPUT); //step
  pinMode(PB5, OUTPUT); //dir

  pinMode(PB15, OUTPUT); //sleep
  pinMode(PA8, OUTPUT); //reset
  pinMode(PA9, OUTPUT); //step
  pinMode(PA10, OUTPUT); //dir

  // pinMode(PC13, OUTPUT); //sleep
  // pinMode(PC14, OUTPUT); //reset
  // pinMode(PC15, OUTPUT); //step
  // pinMode(PA0, OUTPUT); //dir

  
  stepper2.setMaxSpeed(MAX_SPEED);
  stepper2.setAcceleration(MAX_ACCELERATION);
  stepper2.setSpeed(400);

  stepper3.setMaxSpeed(MAX_SPEED);
  stepper3.setAcceleration(MAX_ACCELERATION);
  stepper3.setSpeed(400);

  // stepper3.setMaxSpeed(MAX_SPEED);
  // stepper3.setAcceleration(MAX_ACCELERATION);
  // stepper3.setSpeed(-400);
}

void loop() {
  //Set the spinning direction clockwise:
  digitalWrite(PB7, HIGH);
  digitalWrite(PB8, HIGH);

  digitalWrite(PA9, HIGH);
  digitalWrite(PA10, HIGH);

  // digitalWrite(PC15, HIGH);
  // digitalWrite(PA0, HIGH);

  stepper2.setSpeed(400);
  stepper2.runSpeed();

  stepper3.setSpeed(400);
  stepper3.runSpeed();

  // stepper3.setSpeed(-400);
  // stepper3.runSpeed();
}