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
void setUpSerialUart3(){
    Serial3.begin(9600);
}

void sendSerialMonitor(){
    Serial.print(1);
} 



 //4800, 9600, 14400, 19200, 28800, 31250, 38400, 57600, and 115200