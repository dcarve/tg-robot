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
#define DT_BLUETOOTH_DATA 3000

float w1 = 0;  
float w2 = 0;
float w3 = 0;
int w1_delay=0;
int w2_delay=0;
int w3_delay=0;
float direction_angle = 90;
float angular_speed = 0;
int inByte;
float inFloat;
char inChar;


String readString;
char c;

int nextMotorData  = (millis() + DT_TIME_SEND_MOTOR_DATA);
int nextBtData  = (millis() + DT_BLUETOOTH_DATA);

void setup() {
  // Declare pins as output:

  blueToothDriverPins();
  allDriversStep();
  stepperDriver1(); // 0°
  stepperDriver2(); // 120° 
  stepperDriver3(); // -120°

  stepResolution(HALF_STEP);
  setUpSerialMonitor();
  //Serial.begin(9600);
  //Serial3.begin(9600);
  setUpSerialUart3();

  delay(10000);

  Serial.println("*** AT commands mode ***");

}


void loop() {
    while (Serial3.available()) 
  {
    delay(3);  
    c = Serial3.read();
    readString += c; 
  }// end while
  if (readString.length() >0) 
  {  
    Serial.println(readString);  
   readString="";  
  } // end if
}


// void loop ()
// {
//   // from bluetooth to Terminal.
//   while (Serial3.available() > 0)
//   {
//     Serial.println( (char)Serial3.read());
//   }
//   // from termial to bluetooth
//   if (Serial.available())
//     Serial3.write( (char)Serial.read());
// }


//void loop() {



 // if (millis()>=nextBtData){
    //if (Serial3.available()) {

        //Serial.write(BTSerial.read());

        //inByte = Serial3.read();
        //inFloat = Serial3.read();
        //inChar = Serial3.read();
        //Serial.println(inByte);
        //Serial.println(inFloat);
        //Serial.println(inChar);
    //} else {
    //  Serial.println(' ');
    //}
    //Serial3.write(1);

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

//    nextBtData = millis() + DT_BLUETOOTH_DATA;
   
   
//  }

//}


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