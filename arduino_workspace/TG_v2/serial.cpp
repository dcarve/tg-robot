#include <Arduino.h>
#include "serial.h"


void setUpSerialMonitor(){
    Serial.begin(9600);
}

void setUpSerialUart(){
    Serial.begin(9600);
}

void sendSerialMonitor(){
    Serial.print(1);
} 