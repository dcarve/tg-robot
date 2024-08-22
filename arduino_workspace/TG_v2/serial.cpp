#include <Arduino.h>
#include "serial.h"
#include "pinOutIn.h"
#include <HardwareSerial.h>
#include <libmaple/usart.h>
//#include <SoftwareSerial.h>

//#include <HardwareSerial.h>


void setUpSerialMonitor(){
    Serial.begin(9600);
}
void setUpSerialUsart3(){
    Serial3.begin(9600);
}

char* readUsart3(int *timmer, int delta){
  char c;
  char* readString;

  if (millis()>=*timmer){

    if (Serial3.available()) {
        c = Serial3.read();
        readString += c; 
    }

    *timmer = millis() + delta;
    return readString;
  }
}



 //4800, 9600, 14400, 19200, 28800, 31250, 38400, 57600, and 115200