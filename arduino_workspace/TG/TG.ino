#include <Arduino.h>
#include <math.h>
#include <util/atomic.h>
#include "motors_control.h"
#include "encoders.h"
#include "serial_monitor.h"

#define DT_TIME_SAMPLE_RATE_ENCODER 10 // encoder position reading update rate
#define send_serial 1000 // encoder position reading update rate
#define DT_TIME_INCREASE_ENGINE 5  // engine speed update rate, for testing
#define MAX_VALUE_MOTOR 65535  // 16-bit
#define MIN_VALUE_MOTOR 30000 // minimum value for the motor to start
#define HALL_RESOLUTION 341.2 // encoder position conversion constant for rpm
#define DEFAULT_SPEED 0.05 // meters/second


#define INC_VEL 100   //incremento de velocidade para teste

#define rpm_target 100


int pwmValue = MAX_VALUE_MOTOR;
int nextChangeVel  = (millis() + DT_TIME_INCREASE_ENGINE);
int nextChangeSampleRate  = (millis() + DT_TIME_SAMPLE_RATE_ENCODER);
int nextserial  = (millis() + send_serial);
int inc = INC_VEL;

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

int w1 = 0;
int w2 = 0;
int w3 = 0;


float direction_angle = 90;
float angular_speed = 0;



void setup() {

    motorsSetupPins();
    encodersSetupPins();

    Serial.begin(9600);   

    attachInterrupt(digitalPinToInterrupt(PB0), readEncoder1, RISING);
    attachInterrupt(digitalPinToInterrupt(PB10), readEncoder2, RISING);
    attachInterrupt(digitalPinToInterrupt(PA6), readEncoder3, RISING);

    //pinMode(PA11, OUTPUT);

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

        prevPosition_1 = currPosition_1;
        prevPosition_2 = currPosition_2;
        prevPosition_3 = currPosition_3;

        prevRpm_1 = rpm1;
        prevRpm_2 = rpm2;
        prevRpm_3 = rpm3;

        prevTime = currTime;

        //sent_serial_monitor(
        //    millis(),
        //    rpm1,
        //    filterRpm_1,
        //    rpm2,
        //    filterRpm_2,
        //    rpm3,
        //    filterRpm_3
        //);

        nextChangeSampleRate = millis() + DT_TIME_SAMPLE_RATE_ENCODER;
    }
    
    if (millis()>=nextChangeVel){

    //digitalWrite(PA11, HIGH);



        TransformationMatrix(
            &w1,
            &w2,
            &w3,
            direction_angle,
            angular_speed
        );
        sendMotorOutput(w1,w2,w3);

    //pwmWrite(PB6, 40000);
    //pwmWrite(PB7, 0);
    
    //pwmWrite(PB8, MAX_VALUE_MOTOR);
    //pwmWrite(PB9, 0);

    //pwmWrite(PA9, MAX_VALUE_MOTOR);
    //pwmWrite(PA10, 0);

        nextChangeVel = millis() + DT_TIME_INCREASE_ENGINE;

    }
    
    //if (millis()>=nextserial){
    //    Serial.print("millis:");
    //   Serial.print(millis());
    //    Serial.print(",");
    //   Serial.print("w1:");
    //    Serial.print(w1);
    //    Serial.print(",");
    //    Serial.print("w2:");
    //    Serial.print(w2);
    //    Serial.print(",");
    //    Serial.print("w3:");
    //    Serial.print(w3);
    //   Serial.print(",");
    //    Serial.print("direction_angle:");
    //    Serial.print(direction_angle);
    //    Serial.print('\n');

    //    nextserial = millis() + send_serial;

    //}

}

void readEncoder1(){  
    calcEncoder(digitalRead(PB1), &pos_i_1);
}
void readEncoder2(){  
    calcEncoder(digitalRead(PB11), &pos_i_2);
}
void readEncoder3(){  
    calcEncoder(digitalRead(PA7), &pos_i_3);
}