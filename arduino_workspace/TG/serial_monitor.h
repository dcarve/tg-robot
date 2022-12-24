#ifndef SERIAL_MONITOR_H
#define SERIAL_MONITOR_H
#include <Arduino.h>

void sent_serial_monitor(
    long currT,
    long prevT,
    int pos,
    int posPrev,
    int pwm_value,
    float rpm);

#endif