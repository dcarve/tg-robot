#ifndef ENCODERS_H
#define ENCODERS_H
#include <Arduino.h>

const int FULL_STEP = 0;
const int HALF_STEP = 1;
const int QUARTER_STEP = 2;
const int EIGHTH_STEP = 3;
const int SIXTEENTH_STEP = 4;
const int ONE_THIRTY_SECOND_STEP = 5;

void stepResolutionEncoder(int stepResolution, byte* driver_mode);

#endif