#include <Arduino.h>
#include "serial.h"
#include "pinOutIn.h"
#include <HardwareSerial.h>
#include <libmaple/usart.h>

//#include <HardwareSerial.h>



void setUpSerialMonitor(){
    Serial.begin(115200);
}
void setUpSerialUart3(){
    Serial3.begin(38400);
}

void sendSerialMonitor(){
    Serial.print(1);
} 