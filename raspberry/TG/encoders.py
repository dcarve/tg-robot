#include "encoders.h"
improt math
#include <Arduino.h>
#include <util/atomic.h>
#include <stdio.h>

RADIUS_ROBOT = 100  #in milimeters
DEFAULT_SPEED = 400 #mm/second
RADIUS_WHEEL = 42 #mmm - 58mm


FULL_STEP = 1
HALF_STEP = 2
QUARTER_STEP = 4
EIGHTH_STEP = 8
SIXTEENTH_STEP = 16
ONE_THIRTY_SECOND_STEP = 32

def step_resolution_encoder(step_resolution):
    driver_mode = format(int(math.log2(step_resolution)), '03b')
    return driver_mode


/* 
int rpmPerSecondsToStepPerSeconds(int stepResolution, float rpm){
    int stepPerSeconds;

    stepPerSeconds = ((rpm * stepResolution * 200) / 60);
    return (int) stepPerSeconds;
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
 */

int speedLinearToStepPerSeconds(int stepResolution, float speedLinear){
    float rpm;
    int stepPerSeconds;

    rpm = 60*speedLinear/(2*PI*RADIUS_WHEEL);

    stepPerSeconds = ((rpm * stepResolution * 200) / 60);
    return (int) stepPerSeconds;
}

float convert_degrees_to_radians(float degrees){
    float radians;
    radians = (degrees * 71) / 4068;
    return radians;
}

void TransformationMatrixRpm(volatile long *w1, volatile long *w2, volatile long *w3, float linear_speed_percent, float direction_angle, float angular_speed){

    float linear_speed_x = linear_speed_percent * DEFAULT_SPEED * cos(convert_degrees_to_radians(direction_angle));
    float linear_speed_y = linear_speed_percent * DEFAULT_SPEED * sin(convert_degrees_to_radians(direction_angle));

    float a11 = 0;
    float a12 = -2.0/3;
    float a13 = RADIUS_ROBOT/3;
    float a21 = 1/sqrt(3);
    float a22 = 1.0/3;
    float a23 = RADIUS_ROBOT/3;
    float a31 = -1/sqrt(3);
    float a32 = 1.0/3;
    float a33 = RADIUS_ROBOT/3;

    float speedLinear1, speedLinear2, speedLinear3;

    speedLinear1 = (a11 * linear_speed_x) + (a12 * linear_speed_y) + (a13 * angular_speed);
    speedLinear2 = (a21 * linear_speed_x) + (a22 * linear_speed_y) + (a23 * angular_speed);
    speedLinear3 = (a31 * linear_speed_x) + (a32 * linear_speed_y) + (a33 * angular_speed);
    
    //Serial.print("speed linear 1: ");
    //Serial.print(speedLinear1, 4);
    //Serial.print(" , speed linear 2: ");
    //Serial.print(speedLinear2, 4);
    //Serial.print(" , speed linear 3: ");
    //Serial.println(speedLinear3, 4);

    *w1 = speedLinearToStepPerSeconds(ONE_THIRTY_SECOND_STEP, speedLinear1);
    *w2 = speedLinearToStepPerSeconds(ONE_THIRTY_SECOND_STEP, speedLinear2);
    *w3 = speedLinearToStepPerSeconds(ONE_THIRTY_SECOND_STEP, speedLinear3);
     
}

float max_of_three(float a, float b, float c) {
    if (a >= b && a >= c) {
        return a;
    } else if (b >= a && b >= c) {
        return b;
    } else {
        return c;
    }
}

float min_of_three(float a, float b, float c) {
    if (a <= b && a <= c) {
        return a;
    } else if (b <= a && b <= c) {
        return b;
    } else {
        return c;
    }
}


float mapLogarithmic(float value, float inMin, float inMax, float outMin, float outMax) {
    if (value<60.0){value=60.0;}

    float normalized = (value - inMin) / (inMax - inMin);
    float logValue = log10(1 + 9 * normalized);
    float mappedValue = outMax + (outMin - outMax) * (1 - logValue);

    if (mappedValue<0.0){mappedValue=0.0;}    

    return mappedValue;
}



void rgbToDiretionAngleAndMagnitude(char rgb[], float *h, float *l){
    //baseado no algoritmo de conversão de rgb para hsl

    float r, g, b;

    char substr1[4]; // 4 to include the null terminator
    strncpy(substr1, rgb, 3);
    substr1[3] = '\0'; // Null terminate the substring
    r = (float)atoi(substr1);

    char substr2[4];
    strncpy(substr2, rgb + 3, 3);
    substr2[3] = '\0';
    g = (float)atoi(substr2);

    char substr3[4];
    strncpy(substr3, rgb + 6, 3);
    substr3[3] = '\0';
    b = (float)atoi(substr3);

    r = r/255.0;
    g = g/255.0;
    b = b/255.0;

    float max_color = max_of_three(r, g, b);
    float min_color = min_of_three(r, g, b);
    *l = (max_color + min_color) / 2.0;

    if (max_color == min_color){
        *h =  0.0;  //Achromatic
    }  else {
        float  delta = max_color - min_color;
        
        if (max_color == r) {
            *h = (g - b) / delta + ((g < b) ? 6.0 : 0.0);
        } else if (max_color == g) {
            *h = (b - r) / delta + 2.0;
        } else {
            *h = (r - g) / delta + 4.0;
        }
        *h /= 6.0;
    }

    *h = roundf(((360.0 - (*h * 360.0)) * 100.0) / 100.0); // Convert to degrees and invert the x-axis
    *l = roundf(*l * 100.0 * 100.0) / 100.0; // Convert to percentage
    *l = mapLogarithmic(*l, 60.0, 90.0, 100.0, 0.0);
    *l = *l/100;
}