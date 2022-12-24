#include <Arduino.h>
#include <util/atomic.h>
#include "motors_control.h"
#include "encoders.h"
#include "serial_monitor.h"

#define DT_TIME_SAMPLE_RATE_ENCODER 100
#define DT_TIME_INCREASE_MOTOR 5000
#define MAX_VALUE_MOTOR 65000
#define MIN_VALUE_MOTOR 20000
#define INC_VEL 10000

int pwm_value = MIN_VALUE_MOTOR;
int next_change_vel  = (millis() + DT_TIME_INCREASE_MOTOR);  // calc prox toogle de vel
int next_change_sample_rate  = (millis() + DT_TIME_SAMPLE_RATE_ENCODER);  // calc prox toogle de vel
int inc = INC_VEL;   
long prevT = 0;
int posPrev = 0;
volatile int pos_i = 0;
int pulse_number;
byte      Encoder_C1Last;
boolean direction_m;
int pos = 0;

void setup() {

    motorsSetupPins();
    encodersSetupPins();

    Serial.begin(9600);   

    attachInterrupt(digitalPinToInterrupt(PB0), readEncoder,RISING);

}

void loop() {

    if (millis()>=next_change_sample_rate){
        int pos = 0;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
            pos = pos_i;
        }
        long currT = micros();
        float rpm = calc_rpm(currT, prevT, pos, posPrev);

        sent_serial_monitor(currT, prevT, pos, posPrev, pwm_value, rpm);

        posPrev = pos;
        prevT = currT;

        next_change_sample_rate = millis() + DT_TIME_SAMPLE_RATE_ENCODER;
    }

    if (millis()>=next_change_vel){
        pwm_value = pwm_value + inc;
        if (pwm_value < MIN_VALUE_MOTOR){
            inc = -inc;
            pwm_value = MIN_VALUE_MOTOR;
        } else if (pwm_value > MAX_VALUE_MOTOR) {
            inc = -inc;
            pwm_value = MAX_VALUE_MOTOR;
        }
        
        motorsOutput(PB8, PB9, pwm_value, 1);

        next_change_vel = millis() + DT_TIME_INCREASE_MOTOR;

      }

    

}

/* 
void readEncoder(){
    int b = digitalRead(PB1);
    int increment = 0;
    if(b>0){
        increment = 1;
    }
    else {
        increment = -1;
    }
    pos_i = pos_i + increment;
  
}
 */

void readEncoder(){
    pos_i = readEncoderCalc(pos_i);
}

