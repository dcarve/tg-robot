#include <Arduino.h>
#include <math.h>
#include <util/atomic.h>
#include "motors_control.h"
#include "encoders.h"
#include "serial_monitor.h"


#define DT_TIME_SAMPLE_RATE_ENCODER 10 // encoder position reading update rate
#define DT_TIME_INCREASE_ENGINE 100  // engine speed update rate, for testing

//#define RPM_MAX 209.0
//#define RPM_MIN 95.0

//#define INC_VEL 0.01


#define RPM_MAX 20000
#define RPM_MIN 0

//#define RPM_MAX 65535
//#define RPM_MIN 15000

#define INC_VEL 20


int nextChangeVel  = (millis() + DT_TIME_INCREASE_ENGINE);
int nextChangeSampleRate  = (millis() + DT_TIME_SAMPLE_RATE_ENCODER);

long prevTime = 0;
long currTime = micros();

volatile int pos_i_1 = 0;
volatile int pos_i_2 = 0;
volatile int pos_i_3 = 0;

int prevPosition_1 = 0;
int prevPosition_2 = 0;
int prevPosition_3 = 0;

float filterRpm_1 = 0;
float filterRpm_2 = 0;
float filterRpm_3 = 0;

float prevRpm_1 = 0;
float prevRpm_2 = 0;
float prevRpm_3 = 0;

float w1 = 0;  
float w2 = 0;
float w3 = 0;


float direction_angle = 90;
//float direction_angle = 120;
//float direction_angle = 240;
float angular_speed = 0;

int acrescimo_1 = INC_VEL;
int acrescimo_2 = INC_VEL;
int acrescimo_3 = INC_VEL;

float erro1_n=0;
float erro1_n_1=0;
float i1_n=0;
float i1_n_1=0;
float u1=0;

float erro2_n=0;
float erro2_n_1=0;
float i2_n=0;
float i2_n_1=0;
float u2=0;

float erro3_n=0;
float erro3_n_1=0;
float i3_n=0;
float i3_n_1=0;
float u3=0;

int u1_bit=0;

float volt1 = 0.0;
float dc1 = 0.0;
int bits1 = 0;

float volt2 = 0.0;
float dc2 = 0.0;
int bits2 = 0;

float volt3 = 0.0;
float dc3 = 0.0;
int bits3 = 0;

int incomingByte = 0;

void setup() {

    motorsSetupPins();
    encodersSetupPins();

    Serial.begin(9600);   

    attachInterrupt(digitalPinToInterrupt(PB0), readEncoder1, RISING);
    attachInterrupt(digitalPinToInterrupt(PB10), readEncoder2, RISING);
    attachInterrupt(digitalPinToInterrupt(PA6), readEncoder3, RISING);
    
}

void loop() {

    //Leitura da velocidade
    if (millis()>=nextChangeSampleRate){

        currTime = micros();

        int currPosition_1 = 0;  
        int currPosition_2 = 0;  
        int currPosition_3 = 0;


        ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
            currPosition_1 = pos_i_1;
            currPosition_2 = pos_i_2;
            currPosition_3 = pos_i_3;
        };
        
        float rpm1 = calc_rpm(currTime, prevTime, currPosition_1, prevPosition_1);
        float rpm2 = calc_rpm(currTime, prevTime, currPosition_2, prevPosition_2);
        float rpm3 = calc_rpm(currTime, prevTime, currPosition_3, prevPosition_3);

        filterRpm_1 = low_pass_filter_first_order(rpm1, prevRpm_1, filterRpm_1);
        filterRpm_2 = low_pass_filter_first_order(rpm2, prevRpm_2, filterRpm_2);
        filterRpm_3 = low_pass_filter_first_order(rpm3, prevRpm_3, filterRpm_3);

        erro1_n = w1 - filterRpm_1;

        // controler(
        //     &erro1_n,
        //     &erro1_n_1,
        //     &i1_n,
        //     &i1_n_1,
        //     &u1,
        //     currTime,
        //     prevTime
        // );


        prevPosition_1 = currPosition_1;
        prevPosition_2 = currPosition_2;
        prevPosition_3 = currPosition_3;

        prevRpm_1 = rpm1;
        prevRpm_2 = rpm2;
        prevRpm_3 = rpm3;

        prevTime = currTime;

        i1_n = i1_n_1;
        erro1_n_1 = erro1_n;


        Serial.print("{\"millis\":");    
        Serial.print(millis());
        Serial.print(",\"filterRpm_1\":");
        Serial.print(filterRpm_1);
        Serial.print(",\"w1\":");
        Serial.print(w1);
        Serial.print(",\"filterRpm_2\":");
        Serial.print(filterRpm_2);
        Serial.print(",\"w2\":");
        Serial.print(w2);
        Serial.print(",\"filterRpm_3\":");
        Serial.print(filterRpm_3);
        Serial.print(",\"w3\":");
        Serial.print(w3);
        Serial.print("}\n");

        nextChangeSampleRate = millis() + DT_TIME_SAMPLE_RATE_ENCODER;
    }
    


    if (millis()>=nextChangeVel){


        TransformationMatrix(
            &w1,
            &w2,
            &w3,
            direction_angle,
            angular_speed
        );

        w1=150.0;
        w2=150.0;
        w3=150.0;

        bits1 = convertRpmto16bit(w1);
        
        bits2 = convertRpmto16bit(w2);
        
        bits3 = convertRpmto16bit(w3);

                    
        sendMotorOutput(bits1,bits2,bits3);

        nextChangeVel = millis() + DT_TIME_INCREASE_ENGINE;
        

    }

    //sendMotorOutput(u1_bit,0,0);
}

void readEncoder1(){ 
    int b = digitalRead(PB1);
    if(b>0){
        pos_i_1++;
    }
    else {
        pos_i_1--;
    }
}

void readEncoder2(){
    int b = digitalRead(PB11);
    if(b>0){
        pos_i_2++;
    }
    else {
        pos_i_2--;
    }
}

void readEncoder3(){
    int b = digitalRead(PA7);
    if(b>0){
        pos_i_3++;
    }
    else {
        pos_i_3--;
    }
}