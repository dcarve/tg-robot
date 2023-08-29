#include "motors_control.h"

#define RADIUS_ROBOT 0.2  // in meters
#define DEFAULT_SPEED 0.05 // meters/second


float speed_robot = DEFAULT_SPEED;
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
        pwmWrite(in2, 0);
    } else if(direction==0){
        pwmWrite(in1, 0);
        pwmWrite(in2, value);
    }
};

void up_down_motor(int *pwmValue, int *inc, int maxValueMotor, int minValueMotor){
        int send_pwm_value;

        *pwmValue = *pwmValue + *inc;
        if (*pwmValue < minValueMotor){
            *inc = -*inc;
            *pwmValue = minValueMotor;
        } else if (*pwmValue > maxValueMotor) {
            *inc = -*inc;
            *pwmValue = maxValueMotor;
        }
        
        send_pwm_value = *pwmValue;

        motorsOutput(PB8, PB9, send_pwm_value, 1);

}

void TransformationMatrix(int *w1, int *w2, int *w3, float direction_angle, float angular_speed){

    linear_speed_x = speed_robot * cos(direction_angle);
    linear_speed_y = speed_robot * sin(direction_angle);

    a11 = 0;
    a12 = 2/3;
    a13 = radius/3;
    a21 = 1/sqrt(3);
    a22 = 1/3;
    a23 = radius/3;
    a31 = -1/sqrt(3);
    a32 = 1/3;
    a33 = radius/3;


    *w1 = (int) (a11 * linear_speed_x + a12 * linear_speed_y + a13 * angular_speed);
    *w2 = (int) (a21 * linear_speed_x + a22 * linear_speed_y + a23 * angular_speed);
    *w3 = (int) (a31 * linear_speed_x + a32 * linear_speed_y + a33 * angular_speed);
    
}

void sendMotorOutput(int w1, int w2, int w3){

    //motor 1
    if (w1<0) {
        motorsOutput(PB6, PB7, -w1, 0);
    } else {
        motorsOutput(PB6, PB7, w1, 1);
    }

    //motor 2
    if (w2<0) {
        motorsOutput(PB8, PB9, -w2, 0);
    } else {
        motorsOutput(PB8, PB9, w2, 1);
    }

    //motor 3
    if (w3<0 ){
        motorsOutput(PA9, PA10, -w3, 0);
    } else {
        motorsOutput(PA9, PA10, w3, 1);
    }

}
