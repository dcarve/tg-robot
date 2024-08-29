#include <Arduino.h>
#include "serial.h"
#include "pinOutIn.h"
#include <HardwareSerial.h>
#include <libmaple/usart.h>
#include "encoders.h"
//#include <SoftwareSerial.h>

//#include <HardwareSerial.h>


void setUpSerialMonitor(){
    Serial.begin(9600);
}
void setUpSerialUsart3(){
    Serial3.begin(9600);
}

// void readRgbUsart(
//   int bufferSize,
//   int *timmer,
//   int delta,
//   char *inputBuffer,
//   int *bufferIndex,
//   float *angle_degrees, 
//   float *magnitude
// ){

//   if (millis()>=*timmer){


//     while (Serial3.available()) {
//       char incomingChar = Serial3.read();
//       if (incomingChar == '\n') {
//         inputBuffer[*bufferIndex] = '\0';
//         float angle_degrees_axu = *angle_degrees;
//         float magnitude_aux = *magnitude;

//         //char inputBuffer_aux[100] = *inputBuffer;

//         rgbToDiretionAngleAndMagnitude(inputBuffer, &angle_degrees_axu, &magnitude_aux);
//         *angle_degrees = angle_degrees_axu;
//         *magnitude = magnitude_aux;
//         *bufferIndex = 0;
//       } else {
//         if (*bufferIndex < bufferSize - 1) {
//           inputBuffer[*bufferIndex] = incomingChar;
//           *bufferIndex++;
//         } else {
//           bufferIndex = 0;
//         }
//       }
//     }


//     *timmer = millis() + delta;
//   }
// }