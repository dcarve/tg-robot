#ifndef SERIAL_H
#define SERIAL_H
#include <Arduino.h>


void setupSerial();
void setupBluetooth(const char* deviceName);
void sendBluetoothMessageLn(const String& message);
void sendBluetoothMessage(const String& message);
bool BluetoothAvailable();
char readBluetoothMessage();
void sendLogSpeed(
    float direction_angle,
    float linear_speed_percent,
    long w1,
    long w2,
    long w3
);

#endif
