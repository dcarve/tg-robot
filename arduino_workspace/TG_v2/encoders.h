#ifndef ENCODERS_H
#define ENCODERS_H
#include <Arduino.h>

const int FULL_STEP = 1;
const int HALF_STEP = 2;
const int QUARTER_STEP = 4;
const int EIGHTH_STEP = 8;
const int SIXTEENTH_STEP = 16;
const int ONE_THIRTY_SECOND_STEP = 32;

void stepResolutionEncoder(int stepResolution, byte* driver_mode);
int delayMicrosecondsForStep(int stepResolution, float rpm);
float convert_rpm_to_speed_linear(float rpm);
float convert_degrees_to_radians(float degrees);
void TransformationMatrix(float *w1, float *w2, float *w3, float direction_angle, float angular_speed);
float convert_speed_linear_to_rpm(float w);
float max_of_three(float a, float b, float c);
float min_of_three(float a, float b, float c);
void rgbToDiretionAngleAndMagnitude(char rgb[], float *h, float *l);
float mapLogarithmic(float value, float inMin, float inMax, float outMin, float outMax);

#endif