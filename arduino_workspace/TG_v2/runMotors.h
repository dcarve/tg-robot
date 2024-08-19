#ifndef RUNMOTORS_H
#define RUNMOTORS_H
#include <Arduino.h>

void stepResolution(int stepResolution);

void convertRpmToDelays(
    int *w1_delay,
    int *w2_delay,
    int *w3_delay,
    float w1,
    float w2,
    float w3,
    int stepResolution
);

void SetSleep(int motor, byte value);
void SetReset(int motor, byte value);
void SetStep(int motor, byte value);

#endif