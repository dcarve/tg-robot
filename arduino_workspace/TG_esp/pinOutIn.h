#ifndef PINOUTIN_H
#define PINOUTIN_H
#include <Arduino.h>


//https://lastminuteengineers.com/esp32-pinout-reference/

const byte  RESET_ALL = 13;
const byte  RELES = 23;


const byte  SLEEP_1 = ;
const byte  SLEEP_2 = 16;
const byte  SLEEP_3 = 4;

//Driver 1
const byte  DIR_1 = 32;
const byte  STEP_1 = 33;

//Driver 2
const byte  DIR_2 = 25;
const byte  STEP_2 = 26;

//Driver 3
const byte  DIR_3 = 27;
const byte  STEP_3 = 14; 


void allDriversStep();
void reles();
void stepperDriver1();
void stepperDriver2();
void stepperDriver3();


#endif