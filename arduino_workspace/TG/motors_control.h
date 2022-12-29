#ifndef MOTORS_CONTROL_H
#define MOTORS_CONTROL_H
#include <Arduino.h>

void motorsOutput(
    byte in1,
    byte in2,
    int value,
    int direction);

void motorsSetupPins();

void up_down_motor(int *pwmValue, int *inc, int maxValueMotor, int minValueMotor);

#endif