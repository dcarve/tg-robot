#include "encoders.h"
#include <math.h>
#include <Arduino.h>
#include <util/atomic.h>
#include <stdio.h>

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

// float mapLogarithmic(float value) {
//     // Ensure the input value is within the range
//     if (value < 50 || value > 100) {
//         printf("Value out of range!\n");
//         return -1;
//     }

//     // Normalize the input value to a range [0, 1]
//     float normalizedValue = (value - 50) / (100 - 50);
    
//     // Apply logarithmic scaling
//     // Note: log(x) is undefined for x <= 0, so we use a small positive offset
//     float logValue = log(1 + normalizedValue * 9); // log(1 + 9) = log(10) is a reasonable scaling
    
//     // Map the logarithmic value to the range [0, 100]
//     // Normalize logValue to [0, 1] and then map to [100, 0]
//     float result = 100 * (1 - (logValue / log(10))); // log(10) is the max value of logValue

//     return result;
// }

float mapLogarithmic(float value, float inMin, float inMax, float outMin, float outMax) {
    if (value<58.0){
        value=58.0;
    } 

    // Normalize the input value to a 0-1 range

    float normalized = (value - inMin) / (inMax - inMin);
    
    // Apply the logarithmic transformation
    float logValue = log10(1 + 9 * normalized);
    
    // Scale to the output range
    float mappedValue = outMax + (outMin - outMax) * (1 - logValue);
    if (mappedValue<0.0){
        mappedValue=0.0;
    } 
    
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
    *l = roundf(*l * 100.0 * 100.0) / 100.0;  // Convert to percentage
    //*l = map(*l, 50, 100, 100, 0); //remap the value  of 50-100 to 100-0

    *l = mapLogarithmic(*l, 58.0, 90.0, 100.0, 0.0);
    

    //*l = mapLogarithmic(*l);


}