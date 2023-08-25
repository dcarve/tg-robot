#include <Arduino.h>
#include <math.h>
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

#define rpm_target 100


int pwmValue = MAX_VALUE_MOTOR;
int nextChangeVel  = (millis() + DT_TIME_INCREASE_ENGINE);
int nextChangeSampleRate  = (millis() + DT_TIME_SAMPLE_RATE_ENCODER);
int inc = INC_VEL;

long prevTime = 0;
long currTime = micros();
int e_integral = 0;
float kp = 20000;
float kd = 0;
float ki = 1;

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


void setup() {

    motorsSetupPins();
    encodersSetupPins();

    Serial.begin(9600);   

    attachInterrupt(digitalPinToInterrupt(PB0), readEncoder,RISING);
    attachInterrupt(digitalPinToInterrupt(PB10), readEncoder,RISING);
    attachInterrupt(digitalPinToInterrupt(PA6), readEncoder,RISING);

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

        sent_serial_monitor(
            millis(),
            rpm1,
            filterRpm_1,
            rpm2,
            filterRpm_2,
            rpm3,
            filterRpm_3
        );




        float erro = rpm_target - filterRpm_1;
        e_integral = e_integral + erro*(currTime-prevTime);

        
        float u = kp*erro + ki*e_integral;


        int send_pwm_value = (int) fabs(u);

        if (send_pwm_value > MAX_VALUE_MOTOR){
            send_pwm_value=MAX_VALUE_MOTOR;
        }

        motorsOutput(PB8, PB9, send_pwm_value, 1);





        nextChangeSampleRate = millis() + DT_TIME_SAMPLE_RATE_ENCODER;
    }




    //if (millis()>=nextChangeVel){
    //    up_down_motor(&pwmValue, &inc, MAX_VALUE_MOTOR, MIN_VALUE_MOTOR);
    //    nextChangeVel = millis() + DT_TIME_INCREASE_MOTOR;
    //}

}

void readEncoder(){  

    readEncoderCalcEngine1(&pos_i_1);
    readEncoderCalcEngine2(&pos_i_2);
    readEncoderCalcEngine3(&pos_i_3);

}