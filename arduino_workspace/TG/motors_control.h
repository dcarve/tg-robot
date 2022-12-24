#ifndef MOTORS_CONTROL_H
#define MOTORS_CONTROL_H
#include <Arduino.h>

void motorsOutput(
    byte in1,
    byte in2,
    int value,
    int direction);

void motorsSetupPins();

#endif