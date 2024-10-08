#include <Arduino.h>
#include <math.h>
#include "pinOutIn.h"
#include "stepperMotors.h"
#include "encoders.h"
#include "serial.h"
#include <AccelStepper.h>
//#include "BluetoothSerial.h"
#include "btCommands.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


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

//BluetoothSerial SerialBT;

void setup() {
  allDriversStep();
  reles();
  stepperDriver1(); // 0°
  stepperDriver2(); // 120° 
  stepperDriver3(); // -120°
  switchOffReles();

  //Serial.begin(115200);
  setupSerial();
  setupBluetooth("RoboTG_espmodule");
  //SerialBT.begin("RoboTG_espmodule");
  //Serial.println("The device started, now you can pair it with bluetooth!");

  stepper1.setMaxSpeed(MAX_SPEED);  // Max speed (steps per second)
  stepper1.setAcceleration(MAX_ACCELERATION);  // Acceleration (steps per second^2)

  stepper2.setMaxSpeed(MAX_SPEED);
  stepper2.setAcceleration(MAX_ACCELERATION);

  stepper3.setMaxSpeed(MAX_SPEED);
  stepper3.setAcceleration(MAX_ACCELERATION);

}

void btCommandSerial(){

  //while (SerialBT.available()) {
  while (BluetoothAvailable()) {
    //char incomingChar = SerialBT.read();
    char incomingChar = readBluetoothMessage();

    if (incomingChar == '\n') {
      inputBuffer[bufferIndex] = '\0'; // Null-terminate the string
      
      char substr[4];
      strncpy(substr, inputBuffer, 3);
      substr[3] = '\0';
      char str_compare[] = "cmd";
      str_compare[3] = '\0';

      if (strcmp(substr, str_compare) == 0) {
          char command[2];
          strncpy(command, inputBuffer+3, 3);
          int command_int = (int)atoi(command);
          
          runBtCommands(command_int);

      } else {
        rgbToDiretionAngleAndMagnitude(inputBuffer, &direction_angle, &linear_speed_percent);
        sendLogSpeed(direction_angle, linear_speed_percent, w1, w2, w3);
      }
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

  SetMotorResetAll(HIGH);
 
  if (w1==0){SetMotorSleep(1, LOW);} else {SetMotorSleep(1, HIGH);}
  if (w2==0){SetMotorSleep(2, LOW);} else {SetMotorSleep(2, HIGH);}
  if (w3==0){SetMotorSleep(3, LOW);} else {SetMotorSleep(3, HIGH);}


  if (millis()>=nextBtData){
    btCommandSerial();  
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

