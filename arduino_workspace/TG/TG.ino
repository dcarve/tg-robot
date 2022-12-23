#include <Arduino.h>
#include "pwm.h"
//void setup() {
//  pinMode(PC13, OUTPUT);
//}
// 
//void loop() {
//  digitalWrite(PC13, HIGH);
//  delay(1000);
//  digitalWrite(PC13, LOW);
//  delay(1000);
//}
//

unsigned long myTime;

void setup() {
    pinMode(PB7, PWM);
    pinMode(PB8, PWM);
    pinMode(PB0, INPUT);
    pinMode(PB1, INPUT);
    Serial.begin(9600);
}

void loop() {
    myTime = millis();
    int encoderA = analogRead(PB0);
    int encoderB = analogRead(PB1);

    Serial.print("millis:  ");
    Serial.print(myTime);
    Serial.print("  encoderA: ");
    Serial.print(encoderA);
    Serial.print("  encoderB: ");
    Serial.println(encoderB);


    set_pwm_value(PB7, 4000);
    set_pwm_value(PB8, -1);    
}


/* void loop() {
    int sensorValue = analogRead(PB0);
    int sensorValue = analogRead(PB0);
    Serial.println(4000);
    set_pwm_value(PB7, 4000);
    set_pwm_value(PB8, -1);

    delay(2000);
    
    pwmWrite(PB7, -1);
    pwmWrite(PB8, -1);

    delay(1000);
    
    pwmWrite(PB7, -1);
    pwmWrite(PB8, 4000);

    delay(2000);
    
    pwmWrite(PB7, -1);
    pwmWrite(PB8, -1);

    delay(1000);
    
}
 */
