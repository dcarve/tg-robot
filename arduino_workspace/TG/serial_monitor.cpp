#include <Arduino.h>
#include "serial_monitor.h"

void sent_serial_monitor(
    long millis,
    float rpm1,
    float filterRpm_1
    //float rpm2,
    //float filterRpm_2,
    //float rpm3,
    //float filterRpm_3
){

   
    //Serial.print("millis:");
    Serial.print(millis);
    Serial.print("\t");
    //Serial.print("rpm1:");
    Serial.print(rpm1);
    Serial.print("\t");
    //Serial.print("filterRpm_1:");
    Serial.print(filterRpm_1);
    //Serial.print("\t");
    //Serial.print("rpm2:");
    //Serial.print(rpm2);
    //Serial.print("\t");
    //Serial.print("filterRpm_2:");
    //Serial.print(filterRpm_2);
    //Serial.print("\t");
    //Serial.print("rpm3:");
    //Serial.print(rpm3);
    //Serial.print("\t");
    //Serial.print("filterRpm_3:");
    //Serial.print(filterRpm_3);
    Serial.print('\n');
}