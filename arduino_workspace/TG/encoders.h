#ifndef ENCODERS_H
#define ENCODERS_H
#include <Arduino.h>

void encodersSetupPins();
float calc_rpm(long currT, long prevT, int pos, int posPrev);
float low_pass_filter_first_order(float currRpm, float prevRpm, float prevFilterRpm);

#endif