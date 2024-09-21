import RPi.GPIO as GPIO
from run_motors import motors
import pin_out_in
import encoders
#from encoders import *
#from run_motors import *
#from serial import *

#from RpiMotorLib import RpiMotorLib
#sudo pip3 install rpimotorlib

import time

GPIO.setmode(GPIO.BCM)

GPIO = pin_out_in.setup_pin_driver1(GPIO)  #0º
GPIO = pin_out_in.setup_pin_driver2(GPIO)  #120º
GPIO = pin_out_in.setup_pin_driver3(GPIO)  #-120º
GPIO = pin_out_in.setup_pin_step_resulotion(GPIO)

motors = motorSetup(GPIO)
motors.step_resolution_output(encoders.ONE_THIRTY_SECOND_STEP)

motors.set_motor_reset(1, GPIO.HIGH)
motors.set_motor_reset(2, GPIO.HIGH)
motors.set_motor_reset(3, GPIO.HIGH)

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

step_resolution_pins = (
        pin_out_in.M0,
        pin_out_in.M1,
        pin_out_in.M2
    )




#stepper1 = RpiMotorLib.A4988Nema(DIR_1, STEP_1, step_resolution_pins, "DRV8825")
#stepper2 = RpiMotorLib.A4988Nema(DIR_2, STEP_2, step_resolution_pins, "DRV8825")
#stepper3 = RpiMotorLib.A4988Nema(DIR_3, STEP_3, step_resolution_pins, "DRV8825")


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



# void readUsartConvertRgbToAngleAndMagnitude(){

#   while (Serial3.available()) {
#     char incomingChar = Serial3.read();

#     if (incomingChar == '\n') {
#       inputBuffer[bufferIndex] = '\0'; // Null-terminate the string
#       rgbToDiretionAngleAndMagnitude(inputBuffer, &direction_angle, &linear_speed_percent);

#       Serial.print("direction_angle: ");
#       Serial.print(direction_angle, 4);
#       Serial.print(" , linear_speed_percent: ");
#       Serial.println(linear_speed_percent, 4);

        
#       Serial.print("step per seconds 1: ");
#       Serial.print(w1);
#       Serial.print("    , step per seconds 2: ");
#       Serial.print(w2);
#       Serial.print("    , step per seconds 3: ");
#       Serial.println(w3);
      
#       bufferIndex = 0;
#     } else {
#       if (bufferIndex < bufferSize - 1) {
#         inputBuffer[bufferIndex] = incomingChar;
#         bufferIndex++;
#       } else {
#         bufferIndex = 0; // Reset the buffer index
#       }
#     }
#   }
# }

def sleep_wheen_stopped(w1, w2, w3):
    if w1==0:
        motors.set_motor_sleep(1, GPIO.LOW)
    else:
        motors.set_motor_sleep(1, GPIO.HIGH)

    if w2==0:
        motors.set_motor_sleep(2, GPIO.LOW)
    else:
        motors.set_motor_sleep(2, GPIO.HIGH)

    if w3==0:
        motors.set_motor_sleep(3, GPIO.LOW)
    else:
        motors.set_motor_sleep(3, GPIO.HIGH)



while True:
  sleep_wheen_stopped(1, 1, 1)
  
  GPIO.output(DIR_1, GPIO.HIGH)
  
  GPIO.output(DIR_1, GPIO.HIGH)
  time.sleep(0.5)
  GPIO.output(DIR_1, GPIO.LOW)
  time.sleep(0.5)
  
  """
 
  sleep_wheen_stopped(w1, w2, w3)

  if time.time() >= nextBtData:
    readUsartConvertRgbToAngleAndMagnitude()
    nextBtData = time.time() + DT_BLUETOOTH_DATA
  

  if time.time() >= nextCalcSpeed:

    #w1, w2, w3 = transformation_matrix_rpm(
    #   linear_speed_percent,
    #   direction_angle,
    #   angular_speed
    #)


    w1 = int(linear_speed_percent * 25600)
    w2 = int(linear_speed_percent * 25600)
    w3 = int(linear_speed_percent * 25600)

    nextCalcSpeed = time.time() + DT_CALC_SPEED


  if (time.time()>=nextMotorData):
    #stepper1.setSpeed(w1);
    #stepper2.setSpeed(w2);
    #stepper3.setSpeed(w3);
    nextMotorData = time.time() + DT_TIME_SEND_MOTOR_DATA
  

  #stepper1.runSpeed();
  #stepper2.runSpeed();
  #stepper3.runSpeed();


  """

