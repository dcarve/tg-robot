#ifndef MOTORS_CONTROL_H
#define MOTORS_CONTROL_H
#include <Arduino.h>


void motorsSetupPins();

void motorsOutput(
    byte in1,
    byte in2,
    int value,
    int direction);

void TransformationMatrix(int *w1, int *w2, int *w3, float direction_angle, float angular_speed);
void sendMotorOutput(int w1, int w2, int w3);
float conversor_16bit_rpm(int bit16);
int conversor_rpm_to_16bit(float rpm);
float convert_speed_linear_to_rpm(float w);
float convert_rpm_to_speed_linear(float rpm);
float convert_degrees_to_radians(float degrees);
void controler(float *erro_n, float *erro_n_1, float *i_n, float *i_n_1, float *u, int currT, int prevT);

#endif