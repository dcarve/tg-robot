#ifndef ENCODERS_H
#define ENCODERS_H
#include <Arduino.h>

void encodersSetupPins();
int readEncoderCalc(int pos_i);
float calc_rpm(long currT, long prevT, int pos, int posPrev);

#endif