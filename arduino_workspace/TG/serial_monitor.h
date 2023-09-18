#ifndef SERIAL_MONITOR_H
#define SERIAL_MONITOR_H
#include <Arduino.h>

void sent_serial_monitor(
    long millis,
    float rpm1,
    float filterRpm_1
    //float rpm2,
    //float filterRpm_2,
    //float rpm3,
    //float filterRpm_3
);

#endif