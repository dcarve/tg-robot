#ifndef ENCODERS_H
#define ENCODERS_H
#include <Arduino.h>

const int FULL_STEP = 1;
const int HALF_STEP = 2;
const int QUARTER_STEP = 4;
const int EIGHTH_STEP = 8;
const int SIXTEENTH_STEP = 16;
const int ONE_THIRTY_SECOND_STEP = 32;

int speedLinearToStepPerSeconds(int stepResolution, float speedLinear);

float convert_degrees_to_radians(float degrees);
void TransformationMatrixRpm(
    volatile long *w1,
    volatile long *w2,
    volatile long *w3,
    float linear_speed_percent,
    float direction_angle,
    float angular_speed
);

float maxOfThree(float a, float b, float c);
float minOfThree(float a, float b, float c);
void rgbToDiretionAngleAndMagnitude(char rgb[], float *h, float *l);
float mapLogarithmic(
    float value,
    float inMin,
    float inMax,
    float outMin,
    float outMax
);

#endif