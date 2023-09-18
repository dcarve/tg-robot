#include <Arduino.h>
#include <util/atomic.h>
#include "motors_control.h"
#include "encoders.h"
#include "serial_monitor.h"

#define DT_TIME_SAMPLE_RATE_ENCODER 10 // encoder position reading update rate
#define DT_TIME_INCREASE_ENGINE 50  // engine speed update rate, for testing
#define MAX_VALUE_MOTOR 65535  // 16-bit
#define MIN_VALUE_MOTOR 10000 // minimum value for the motor to start
#define HALL_RESOLUTION 341.2 // encoder position conversion constant for rpm


#define INC_VEL 100   //incremento de velocidade para teste


int pwmValue = MAX_VALUE_MOTOR;
int nextChangeVel  = (millis() + DT_TIME_INCREASE_ENGINE);
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
        float rpm2 = 0;    // teste velocidade método ruim,  não usar no produto final


        ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
            pos = pos_i;

            rpm2 = rpm_i;  // teste velocidade método ruim,  não usar no produto final
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
        nextChangeVel = millis() + DT_TIME_INCREASE_ENGINE;
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




    pos_i = readEncoderCalc(pos_i);

    // método de calcular velocidade usando a diferença de tempo entre clocks para calcular a velocidade

    //
    
    long currT = micros();
    float deltaT = ((float) (currT - prevT_i))/1.0e6;
    velocity_i = increment/deltaT;
    rpm_i = 60*velocity_i/HALL_RESOLUTION;
    prevT_i = currT;



    

}