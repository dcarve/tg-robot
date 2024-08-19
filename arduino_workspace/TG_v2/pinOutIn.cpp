#include "pinOutIn.h"
#include <math.h>
#include <Arduino.h>

// pins A11, A12, A13, B3, B4

void blueToothDriverPins(){
    pinMode(RX_STM, INPUT);   //RX - TXD
    pinMode(TX_STM, OUTPUT);  //TX - RXD
};

void allDriversStep(){ 
    pinMode(M2, OUTPUT); //M2
    pinMode(M1, OUTPUT); //M1
    pinMode(M0, OUTPUT); //M0
};

void stepperDriver1(){ 
    pinMode(DIR_1, OUTPUT); //DIR
    pinMode(STEP_1, OUTPUT);  //SETP
    pinMode(SLEEP_1, OUTPUT);  //SLEEP
    pinMode(RESET_1, OUTPUT); //RESET
};

void stepperDriver2(){
    pinMode(DIR_2, OUTPUT); //DIR 
    pinMode(STEP_2, OUTPUT); //SETP
    pinMode(SLEEP_2, OUTPUT);  //SLEEP
    pinMode(RESET_2, OUTPUT); //RESET
};

void stepperDriver3(){
    pinMode(DIR_3, OUTPUT); //DIR
    pinMode(STEP_3, OUTPUT); //SETP
    pinMode(SLEEP_3, OUTPUT); //SLEEP
    pinMode(RESET_3, OUTPUT);  //RESET
};

void setup_teste_pins(){

  pinMode(M2, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(DIR_1, OUTPUT);
  pinMode(STEP_1, OUTPUT);
  pinMode(SLEEP_1, OUTPUT);
  pinMode(RESET_1, OUTPUT);
  pinMode(PA11, OUTPUT);
  pinMode(PA12, OUTPUT);
  pinMode(PA15, OUTPUT);
  pinMode(PB3, OUTPUT);
  pinMode(PB4, OUTPUT);
  pinMode(DIR_2, OUTPUT);
  pinMode(STEP_2, OUTPUT);
  pinMode(SLEEP_2, OUTPUT);
  pinMode(RESET_2, OUTPUT);
  pinMode(PB9, OUTPUT);
  pinMode(RX_STM, OUTPUT);
  pinMode(TX_STM, OUTPUT);
  pinMode(PB1, OUTPUT);
  pinMode(PB0, OUTPUT);
  pinMode(PA7, OUTPUT);
  pinMode(PA6, OUTPUT);
  pinMode(PA5, OUTPUT);
  pinMode(PA4, OUTPUT);
  pinMode(PA3, OUTPUT);
  pinMode(PA2, OUTPUT);
  pinMode(PA1, OUTPUT);
  pinMode(RESET_3, OUTPUT);
  pinMode(SLEEP_3, OUTPUT);
  pinMode(STEP_3, OUTPUT);
  pinMode(DIR_3, OUTPUT);
};

void run_teste(){

  delayMicroseconds(3000000);

  digitalWrite(M2, LOW);
  digitalWrite(M1, HIGH);
  digitalWrite(M0, LOW);
  digitalWrite(DIR_1, HIGH);
  digitalWrite(STEP_1, LOW);
  digitalWrite(SLEEP_1, HIGH);
  digitalWrite(RESET_1, LOW);
  digitalWrite(PA11, HIGH);
  digitalWrite(PA12, LOW);
  digitalWrite(PA15, HIGH);
  digitalWrite(PB3, LOW);
  digitalWrite(PB4, HIGH);
  digitalWrite(DIR_2, LOW);
  digitalWrite(STEP_2, HIGH);
  digitalWrite(SLEEP_2, LOW);
  digitalWrite(RESET_2, HIGH);
  digitalWrite(PB9, LOW);
  digitalWrite(RX_STM, HIGH);
  digitalWrite(TX_STM, LOW);
  digitalWrite(PB1, HIGH);
  digitalWrite(PB0, LOW);
  digitalWrite(PA7, HIGH);
  digitalWrite(PA6, LOW);
  digitalWrite(PA5, HIGH);
  digitalWrite(PA4, LOW);
  digitalWrite(PA3, HIGH);
  digitalWrite(PA2, LOW);
  digitalWrite(PA1, HIGH);
  digitalWrite(RESET_3, LOW);
  digitalWrite(SLEEP_3, HIGH);
  digitalWrite(STEP_3, LOW);
  digitalWrite(DIR_3, HIGH);

  delayMicroseconds(3000000);

  digitalWrite(M2, HIGH);
  digitalWrite(M1, LOW);
  digitalWrite(M0, HIGH);
  digitalWrite(DIR_1, LOW);
  digitalWrite(STEP_1, HIGH);
  digitalWrite(SLEEP_1, LOW);
  digitalWrite(RESET_1, HIGH);
  digitalWrite(PA11, LOW);
  digitalWrite(PA12, HIGH);
  digitalWrite(PA15, LOW);
  digitalWrite(PB3, HIGH);
  digitalWrite(PB4, LOW);
  digitalWrite(DIR_2, HIGH);
  digitalWrite(STEP_2, LOW);
  digitalWrite(SLEEP_2, HIGH);
  digitalWrite(RESET_2, LOW);
  digitalWrite(PB9, HIGH);
  digitalWrite(RX_STM, LOW);
  digitalWrite(TX_STM, HIGH);
  digitalWrite(PB1, LOW);
  digitalWrite(PB0, HIGH);
  digitalWrite(PA7, LOW);
  digitalWrite(PA6, HIGH);
  digitalWrite(PA5, LOW);
  digitalWrite(PA4, HIGH);
  digitalWrite(PA3, LOW);
  digitalWrite(PA2, HIGH);
  digitalWrite(PA1, LOW);
  digitalWrite(RESET_3, HIGH);
  digitalWrite(SLEEP_3, LOW);
  digitalWrite(STEP_3, HIGH);
  digitalWrite(DIR_3, LOW);
};
