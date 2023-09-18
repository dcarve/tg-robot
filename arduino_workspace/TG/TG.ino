#include <Arduino.h>
#include <math.h>
#include <util/atomic.h>
#include "motors_control.h"
#include "encoders.h"
#include "serial_monitor.h"



#include "pwm.h"

#include <libmaple/libmaple_types.h>
#include <libmaple/timer.h>

#include "boards.h"
#include "io.h"



#define DT_TIME_SAMPLE_RATE_ENCODER 10 // encoder position reading update rate
#define DT_TIME_INCREASE_ENGINE 5  // engine speed update rate, for testing
#define MAX_VALUE_MOTOR 65535  // 16-bit
#define MIN_VALUE_MOTOR 10000 // minimum value for the motor to start

#define INC_VEL 1   //incremento de velocidade para teste

#define rpm_target 100



int pwmValue = MAX_VALUE_MOTOR;
int nextChangeVel  = (millis() + DT_TIME_INCREASE_ENGINE);
int nextChangeSampleRate  = (millis() + DT_TIME_SAMPLE_RATE_ENCODER);
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

int w1 = MIN_VALUE_MOTOR;  
int w2 = MIN_VALUE_MOTOR;
int w3 = MIN_VALUE_MOTOR;


float direction_angle = 90;
//float direction_angle = 120;
//float direction_angle = 240;
float angular_speed = 0;

int acrescimo_1 = 1;
int acrescimo_2 = 1;
int acrescimo_3 = 1;


//uint32_t channel_11;
//uint32_t channel_12;
//HardwareTimer *MyTim_11;
//HardwareTimer *MyTim_12;

void setup() {

    motorsSetupPins();
    encodersSetupPins();

    Serial.begin(9600);   

    attachInterrupt(digitalPinToInterrupt(PB0), readEncoder1, RISING);
    attachInterrupt(digitalPinToInterrupt(PB10), readEncoder2, RISING);
    attachInterrupt(digitalPinToInterrupt(PA6), readEncoder3, RISING);
    

    //TIM_TypeDef *Instance_11 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PB6), PinMap_PWM);
    //channel_11 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PB6), PinMap_PWM));

    //TIM_TypeDef *Instance_12 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PB7), PinMap_PWM);
    //channel_12 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PB7), PinMap_PWM));


    //HardwareTimer *MyTim_11 = new HardwareTimer(Instance_11);
    //HardwareTimer *MyTim_12 = new HardwareTimer(Instance_12);

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


        Serial.print("{\"millis\":");    
        Serial.print(millis());
        Serial.print(",\"rpm1\":");
        Serial.print(rpm1);
        Serial.print(",\"filterRpm_1\":");
        Serial.print(filterRpm_1);
        Serial.print(",\"w1\":");
        Serial.print(w1);
        Serial.print(",\"rpm2\":");
        Serial.print(rpm2);
        Serial.print(",\"filterRpm_2\":");
        Serial.print(filterRpm_2);
        Serial.print(",\"w2\":");
        Serial.print(w2);
        Serial.print(",\"rpm3\":");
        Serial.print(rpm3);
        Serial.print(",\"filterRpm_3\":");
        Serial.print(filterRpm_3);
        Serial.print(",\"w3\":");
        Serial.print(w3);
        Serial.print("}\n");

        nextChangeSampleRate = millis() + DT_TIME_SAMPLE_RATE_ENCODER;
    }
    
    if (millis()>=nextChangeVel){



        w1 = w1 + acrescimo_1;
        if (w1 < MIN_VALUE_MOTOR){
            acrescimo_1 = -acrescimo_1;
            w1 = MIN_VALUE_MOTOR;
        } else if (w1 > MAX_VALUE_MOTOR) {
            acrescimo_1 = -acrescimo_1;
            w1 = MAX_VALUE_MOTOR;
        }


        w2 = w2 + acrescimo_2;
        if (w2 < MIN_VALUE_MOTOR){
            acrescimo_2 = -acrescimo_2;
            w2 = MIN_VALUE_MOTOR;
        } else if (w2 > MAX_VALUE_MOTOR) {
            acrescimo_2 = -acrescimo_2;
            w2 = MAX_VALUE_MOTOR;
        }


        w3 = w3 + acrescimo_3;
        if (w3 < MIN_VALUE_MOTOR){
            acrescimo_3 = -acrescimo_3;
            w3 = MIN_VALUE_MOTOR;
        } else if (w3 > MAX_VALUE_MOTOR) {
            acrescimo_3 = -acrescimo_3;
            w3 = MAX_VALUE_MOTOR;
        }


        //TransformationMatrix(
        //    &w1,
        //    &w2,
        //    &w3,
        //    direction_angle,
        //    angular_speed
        //);
        
        
        //MyTim_11->setPWM(channel_11, PB6, 5, 10);
        //digitalWrite(PB7, LOW);

        sendMotorOutput(w1,w2,w3);

        //sendMotorOutput(65000, 65000, 65000);

        nextChangeVel = millis() + DT_TIME_INCREASE_ENGINE;

    }
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