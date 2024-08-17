#ifndef PINOUTIN_H
#define PINOUTIN_H
#include <Arduino.h>

const byte  TX_STM = PB10;
const byte  RX_STM = PB11;

const byte  M2 = PB12;
const byte  M1 = PB13;
const byte  M0 = PB14;

//Driver 1
const byte  DIR_1 = PB15;
const byte  SETP_1 = PA8;
const byte  SLEEP_1 = PA9;
const byte  RESET_1 = PA10;

//Driver 2
const byte  DIR_2 = PB5;
const byte  SETP_2 = PB6; 
const byte  SLEEP_2 = PB7;
const byte  RESET_2 = PB8;

//Driver 3
const byte  DIR_3 = PC13;
const byte  SETP_3 = PC14;
const byte  SLEEP_3 = PC15;
const byte  RESET_3 = PA0;

void blueToothDriverPins();
void allDriversStep();
void stepperDriver1();
void stepperDriver2();
void stepperDriver3();

void setup_teste_pins();
void run_teste();

#endif