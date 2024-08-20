#include <Arduino.h>
#include "serial.h"
#include "pinOutIn.h"
//#include <HardwareSerial.h>



void setUpSerialMonitor(){
    Serial.begin(9600);
}
void setUpSerialUart3(){
    Serial3.begin(4800);
}


void setUpSerialUart(){
    Serial.begin(9600);
}

void sendSerialMonitor(){
    Serial.print(1);
} 