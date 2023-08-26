#include "motors_control.h"

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
