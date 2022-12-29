#include <Arduino.h>
#include "serial_monitor.h"

void sent_serial_monitor(
    long currT,
    long prevT,
    int pos,
    int posPrev,
    int pwm_value,
    float rpm,
    float rpm2,
    long millis){
    
    int encoderA = digitalRead(PB0);
    int encoderB = digitalRead(PB1);

    //Serial.print("encoderA:");
    //Serial.print(encoderA);
    //Serial.print(",");
    //Serial.print("encoderB:");
    //Serial.print(encoderB);
    //Serial.print(",");
    //Serial.print("currT:");
    //Serial.print(currT);
    //Serial.print(",");
    //Serial.print("prevT:");
    //Serial.print(prevT);
    //Serial.print(",");
    //Serial.print("pos:");
    //Serial.print(pos);
    //Serial.print(",");
    //Serial.print("posPrev:");
    //Serial.print(posPrev);
    //Serial.print(",");
    //Serial.print("posPrev:");
    //Serial.print(posPrev);
    //Serial.print(",");
    //Serial.print("pwm_value:");
    //Serial.print(pwm_value/1000);
    //Serial.print(",");
    Serial.print("millis:");
    Serial.print(millis);
    Serial.print(",");
    Serial.print("rpm:");
    Serial.print(rpm);
    Serial.print(",");
    Serial.print("rpm2:");
    Serial.println(rpm2);
}