#include "pinOutIn.h"
#include <math.h>
#include <Arduino.h>

void allDriversStep(){ 
    pinMode(RESET_ALL, OUTPUT);
};

void reles(){ 
    pinMode(RELES, OUTPUT); 
};

void stepperDriver1(){ 
    pinMode(DIR_1, OUTPUT); //DIR
    pinMode(STEP_1, OUTPUT);  //SETP
    pinMode(SLEEP_1, OUTPUT);  //SLEEP
};

void stepperDriver2(){
    pinMode(DIR_2, OUTPUT); //DIR 
    pinMode(STEP_2, OUTPUT); //SETP
    pinMode(SLEEP_2, OUTPUT);  //SLEEP
};

void stepperDriver3(){
    pinMode(DIR_3, OUTPUT); //DIR
    pinMode(STEP_3, OUTPUT); //SETP
    pinMode(SLEEP_3, OUTPUT); //SLEEP
};