#ifndef SERIAL_H
#define SERIAL_H
#include <Arduino.h>

void setUpSerialMonitor();
void setUpSerialUsart3();
char* readUsart3(int *timmer, int delta);

#endif