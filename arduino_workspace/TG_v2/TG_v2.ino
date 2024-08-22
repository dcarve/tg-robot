#include <Arduino.h>
#include <math.h>
#include <util/atomic.h>
#include "pinOutIn.h"
#include "runMotors.h"
#include "encoders.h"
#include "serial.h"
#include <HardwareSerial.h>
#include <libmaple/usart.h>


#define DT_TIME_SEND_MOTOR_DATA 1000
#define DT_BLUETOOTH_DATA 100

float w1 = 0;  
float w2 = 0;
float w3 = 0;
int w1_delay=0;
int w2_delay=0;
int w3_delay=0;
float direction_angle = 90;
float angular_speed = 0;
String readString;

// String readString;
// char c;

int deltatBtData = DT_BLUETOOTH_DATA;
int nextMotorData  = (millis() + DT_TIME_SEND_MOTOR_DATA);
int nextBtData  = (millis() + deltatBtData);

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

}




void loop() {

  readString = readUsart3(&nextBtData, deltatBtData);
  if (readString.length() >0) 
    {  
      Serial.println(readString);  
    readString="";  
    }


}



// void loop() {
//   // Get the current time
//   unsigned long currentMillis = millis();

//   // Non-blocking check for serial data at regular intervals
//   if (currentMillis - previousMillis >= interval) {
//     previousMillis = currentMillis;  // Update the last time we checked serial

//     // Check if any data is available on the serial port
//     if (Serial.available() > 0) {
//       // Read the incoming data and print it to the serial monitor
//       String incomingData = Serial.readString();
//       Serial.println("Received: " + incomingData);
//     }
//   }

//   // Other non-blocking code can go here
//   // For example, sensor readings, LED blinking, etc.
// }




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