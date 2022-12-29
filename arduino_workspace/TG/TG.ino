#include <Arduino.h>
#include <util/atomic.h>
#include "motors_control.h"
#include "encoders.h"
#include "serial_monitor.h"

#define DT_TIME_SAMPLE_RATE_ENCODER 10
#define DT_TIME_INCREASE_MOTOR 50
#define MAX_VALUE_MOTOR 65535
#define MIN_VALUE_MOTOR 10000
#define INC_VEL 100
#define HALL_RESOLUTION 341.2

int pwmValue = MAX_VALUE_MOTOR;
int nextChangeVel  = (millis() + DT_TIME_INCREASE_MOTOR);
int nextChangeSampleRate  = (millis() + DT_TIME_SAMPLE_RATE_ENCODER);
int inc = INC_VEL;

long prevT = 0;
int posPrev = 0;
volatile int pos_i = 0;
int pos = 0;

volatile float velocity_i = 0;
volatile float rpm_i = 0;
volatile long prevT_i = 0;


void setup() {

    motorsSetupPins();
    encodersSetupPins();

    Serial.begin(9600);   

    attachInterrupt(digitalPinToInterrupt(PB0), readEncoder,RISING);

}

void loop() {

    if (millis()>=nextChangeSampleRate){
        int pos = 0;
        float rpm2 = 0;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
            pos = pos_i;
            rpm2 = rpm_i;
        }
        long currT = micros();
        float rpm = calc_rpm(currT, prevT, pos, posPrev);

        sent_serial_monitor(currT, prevT, pos, posPrev, pwmValue, rpm, rpm2, millis());

        posPrev = pos;
        prevT = currT;

        nextChangeSampleRate = millis() + DT_TIME_SAMPLE_RATE_ENCODER;
    }

    if (millis()>=nextChangeVel){
        up_down_motor(&pwmValue, &inc, MAX_VALUE_MOTOR, MIN_VALUE_MOTOR);
        nextChangeVel = millis() + DT_TIME_INCREASE_MOTOR;
    }

}

void readEncoder(){
    int b = digitalRead(PB1);
    int increment=0;

    if(b>0){
        increment = 1;
    }
    else {
        increment =-1;
    }

    pos_i = pos_i + increment;


    //pos_i = readEncoderCalc(pos_i);


    long currT = micros();
    float deltaT = ((float) (currT - prevT_i))/1.0e6;
    velocity_i = increment/deltaT;
    rpm_i = 60*velocity_i/HALL_RESOLUTION;
    prevT_i = currT;

}