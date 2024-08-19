#include "motors_control.h"
#include <math.h>
#include <Arduino.h>

#define RADIUS_ROBOT 100  // in meters
#define DEFAULT_SPEED 800 // mm/second
#define RADIUS_WHEEL 29 //mmm - 58mm
#define PI 3.141592653589

//tensão máxima na saida dos drivers
//5.86V
//210*5.86/6 = 205.1

#define RPM_MAX 210
#define MAX_VOLTAGE 6.0

float radius = RADIUS_ROBOT;

void motorsSetupPins(){
    //motor 1
    pinMode(PB6, PWM); // cabo claro IN1
    pinMode(PB7, PWM); // cabo escuro IN2

    //motor 2
    pinMode(PB8, PWM); // cabo claro IN1
    pinMode(PB9, PWM);  // cabo escuro IN2
    
    //motor 3
    pinMode(PA9, PWM); // cabo claro IN1
    pinMode(PA10, PWM); // cabo escuro IN2

};



void motorsOutput(
    byte in1,
    byte in2,
    int value,
    int direction){

    
    if (direction==1){
        pwmWrite(in1, value);
        pwmWrite(in2, LOW);
    } else if(direction==0){
        pwmWrite(in1, LOW);
        pwmWrite(in2, value);
    }
};


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


int convertRpmto16bit(float rpm){
    return 0.0001131 * pow(rpm,4) - 0.03064 * pow(rpm,3) + 2.993 * pow(rpm, 2) - 1.257 * rpm + 9017;
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

void sendMotorOutput(int bits1, int bits2, int bits3){

    //motor 1
    if (bits1<0) {
        motorsOutput(PB7, PB6, -bits1, 0);
    } else {
        motorsOutput(PB7, PB6, bits1, 1);
    }

    //motor 2
    if (bits2<0) {
        motorsOutput(PA10, PA9, -bits2, 0);
    } else {
        motorsOutput(PA10, PA9, bits2, 1);
    }

    //motor 3
    if (bits3<0 ){
        motorsOutput(PB9, PB8, -bits3, 0);
    } else {
        motorsOutput(PB9, PB8, bits3, 1);
    }

}




void controler(float *erro_n, float *erro_n_1, float *i_n, float *i_n_1, float *u, int currT, int prevT){

    float kp = 0.003;
    float kd = 0;
    float ki = 0.02;

    float p_n = *erro_n;

    float d_n = (*erro_n - *erro_n_1)/(currT-prevT);

    *i_n = *i_n_1 + (*erro_n + *erro_n_1)*(currT-prevT)/2;

    *u = kp * p_n + kd * d_n +  ki * *i_n;
}