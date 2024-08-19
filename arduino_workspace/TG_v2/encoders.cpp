#include "encoders.h"
#include <math.h>
#include <Arduino.h>
#include <util/atomic.h>

#define RADIUS_ROBOT 100  // in milimeters
#define DEFAULT_SPEED 800 // mm/second
#define RADIUS_WHEEL 34.55 //mmm - 58mm
#define PI 3.141592653589


float radius = RADIUS_ROBOT;


void stepResolutionEncoder(int stepResolution, byte* driver_mode){

    switch (stepResolution) {
        case 1:
            driver_mode[0] = LOW;
            driver_mode[1] = LOW;
            driver_mode[2] = LOW;
            break;
        case 2:
            driver_mode[0] = HIGH;
            driver_mode[1] = LOW;
            driver_mode[2] = LOW;
            break;
        case 4:
            driver_mode[0] = LOW;
            driver_mode[1] = HIGH;
            driver_mode[2] = LOW;
            break;
        case 8:
            driver_mode[0] = HIGH;
            driver_mode[1] = HIGH;
            driver_mode[2] = LOW;
            break;
        case 16:
            driver_mode[0] = LOW;
            driver_mode[1] = LOW;
            driver_mode[2] = HIGH;
            break;
        case 32:
            driver_mode[0] = HIGH;
            driver_mode[1] = LOW;
            driver_mode[2] = HIGH;
            break;  
    }
}


int delayMicrosecondsForStep(int stepResolution, float rpm){
    float delayValue;

    delayValue = (3*pow(10,5))/(2*stepResolution*rpm);

    return (int) delayValue;
}


float convert_rpm_to_speed_linear(float rpm){
    float w;

    w = RADIUS_WHEEL*2*PI*rpm/(60);
    
    return w;
}

float convert_speed_linear_to_rpm(float w){
    float rpm;

    rpm = 60*w/(2*PI*RADIUS_WHEEL);
    
    return rpm;
}


float convert_degrees_to_radians(float degrees){
    float radians;
    radians = (degrees * 71) / 4068;
    return radians;
}


void TransformationMatrix(float *w1, float *w2, float *w3, float direction_angle, float angular_speed){

    float linear_speed_x = DEFAULT_SPEED * cos(convert_degrees_to_radians(direction_angle));
    float linear_speed_y = DEFAULT_SPEED * sin(convert_degrees_to_radians(direction_angle));

    float a11 = 0;
    float a12 = -2.0/3;
    float a13 = radius/3;
    float a21 = 1/sqrt(3);
    float a22 = 1.0/3;
    float a23 = radius/3;
    float a31 = -1/sqrt(3);
    float a32 = 1.0/3;
    float a33 = radius/3;

    float aux1, aux2, aux3;

    aux1 = (a11 * linear_speed_x) + (a12 * linear_speed_y) + (a13 * angular_speed);
    aux2 = (a21 * linear_speed_x) + (a22 * linear_speed_y) + (a23 * angular_speed);
    aux3 = (a31 * linear_speed_x) + (a32 * linear_speed_y) + (a33 * angular_speed);
    
    *w1 = convert_speed_linear_to_rpm(aux1);
    *w2 = convert_speed_linear_to_rpm(aux2);
    *w3 = convert_speed_linear_to_rpm(aux3);
     
}