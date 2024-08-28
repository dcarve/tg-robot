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
String readString;


///variavel da parte do bleutooth LUCAS

char c;
///

int deltatBtData = DT_BLUETOOTH_DATA;
int nextMotorData  = (millis() + DT_TIME_SEND_MOTOR_DATA);
int nextBtData  = (millis() + deltatBtData);


const int bufferSize = 100; // Define the buffer size
char inputBuffer[bufferSize]; // Buffer to store the input string
int bufferIndex = 0; // Index for the buffer

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
  // Check if data is available on the serial port
  while (Serial3.available()) {
    char incomingChar = Serial3.read(); // Read one character
    //Serial.println(incomingChar);

    // If the received character is the newline character, process the input
    if (incomingChar == '\n') {
      inputBuffer[bufferIndex] = '\0'; // Null-terminate the string
      Serial.print("Received: ");
      Serial.println(inputBuffer); // Print the received string

      // Reset the buffer index for the next input
      bufferIndex = 0;
    } else {
      // Store the incoming character in the buffer if there is space
      if (bufferIndex < bufferSize - 1) {
        inputBuffer[bufferIndex] = incomingChar;
        bufferIndex++;
      } else {
        // If the buffer is full, discard the input and print a warning
        Serial.println("Buffer overflow! Input too long.");
        bufferIndex = 0; // Reset the buffer index
      }
    }
  }

  // if (millis()>=nextBtData){
  //   Serial.println("NO DATA");

  //   nextBtData = millis() + deltatBtData;
  // }
  
}
// void loop() {

//   readString = readUsart3(&nextBtData, deltatBtData);
//   if (readString.length() >0) 
//     {  
//       Serial.println(readString);  
//     readString="";  
//    }

//   if (Serial3.available()){

//    c = Serial3.read();
//    readString += c; 
//   }

//   if (readString.length() >0) 
//  {  
//    Serial.println(readString);  
//    readString="";  

// }




// unsigned long previousMillis = 0;
// unsigned long delayStartMillis = 0;
// const long checkInterval = 10;
// const long pseudoDelay = 3;

// bool delayStarted = false;
// String inputString = "";
// bool stringComplete = false;  

// void loop() {
//   unsigned long currentMillis = millis();

//   if (currentMillis - previousMillis >= checkInterval) {
//     previousMillis = currentMillis;


//     if (Serial3.available() && !delayStarted) {
//       delayStartMillis = millis();
//       delayStarted = true;
//     }


//     if (delayStarted && (currentMillis - delayStartMillis >= pseudoDelay)) {
//       while (Serial3.available()) {

//         char incomingByte = Serial3.read();
//         inputString += incomingByte;

        
//         if (incomingByte == '\n') {
//           stringComplete = true;
//           delayStarted = false; 
//           break; 
//         }
//       }
//     }

//     if (stringComplete) {
//       Serial.print(inputString);
//       inputString = "";
//       stringComplete = false;
//     }
//   }
// }


// void loop() {
//     while (Serial3.available()) 
//   {
//     //delay(3);  
//     c = Serial3.read();
//     readString += c; 
//   }// end while
//   if (readString.length() >0) 
//   {  
//     Serial.println(readString);  
//    readString="";  
//   } // end if
// }




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