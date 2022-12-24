#include "motors_control.h"

void motorsSetupPins(){
    //motor 1
    pinMode(PB6, PWM);
    pinMode(PB7, PWM);

    //motor 2
    pinMode(PB8, PWM);
    pinMode(PB9, PWM);  
    
    //motor 3
    pinMode(PA9, PWM);
    pinMode(PA10, PWM);

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
