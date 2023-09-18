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

#define rpm_max 205.1

float radius = RADIUS_ROBOT;

void motorsSetupPins(){
    //motor 1
    pinMode(PB6, OUTPUT); // cabo claro IN1
    pinMode(PB7, OUTPUT); // cabo escuro IN2

    //motor 2
    pinMode(PB8, OUTPUT); // cabo claro IN1
    pinMode(PB9, OUTPUT);  // cabo escuro IN2
    
    //motor 3
    pinMode(PA9, OUTPUT); // cabo claro IN1
    pinMode(PA10, OUTPUT); // cabo escuro IN2

};


// void motorsSetupPinsNew(){


//     TIM_TypeDef *Instance_11 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PB6), PinMap_PWM);
//     uint32_t channel_11 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PB6), PinMap_PWM));

//     TIM_TypeDef *Instance_12 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PB7), PinMap_PWM);
//     uint32_t channel_12 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PB7), PinMap_PWM));



//     TIM_TypeDef *Instance_21 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PB8), PinMap_PWM);
//     uint32_t channel_21 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PB8), PinMap_PWM));

//     TIM_TypeDef *Instance_22 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PB9), PinMap_PWM);
//     uint32_t channel_22 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PB9), PinMap_PWM));



//     TIM_TypeDef *Instance_31 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PA9), PinMap_PWM);
//     uint32_t channel_31 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PA9), PinMap_PWM));

//     TIM_TypeDef *Instance_32 = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PA10), PinMap_PWM);
//     uint32_t channel_32 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PA10), PinMap_PWM));


//     HardwareTimer *MyTim_11 = new HardwareTimer(Instance_11);
//     HardwareTimer *MyTim_12 = new HardwareTimer(Instance_12);
//     HardwareTimer *MyTim_21 = new HardwareTimer(Instance_21);
//     HardwareTimer *MyTim_22 = new HardwareTimer(Instance_22);
//     HardwareTimer *MyTim_31 = new HardwareTimer(Instance_31);
//     HardwareTimer *MyTim_32 = new HardwareTimer(Instance_32);

//     MyTim->setPWM(channel, pin, 5, 10);

// };

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



float conversor_16bit_rpm(int bit16){
    float rpm = (float) (rpm_max * bit16 / 65535);
    return rpm;
}

int conversor_rpm_to_16bit(float rpm){
    int bit16 = (int) (65535 * rpm / rpm_max);
    return bit16;
}

float convert_degrees_to_radians(float degrees){
    float radians;
    radians = (degrees * 71) / 4068;
    return radians;
}


void TransformationMatrix(int *w1, int *w2, int *w3, float direction_angle, float angular_speed){

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
    
    *w1 = conversor_rpm_to_16bit(convert_speed_linear_to_rpm(aux1));
    *w2 = conversor_rpm_to_16bit(convert_speed_linear_to_rpm(aux2));
    *w3 = conversor_rpm_to_16bit(convert_speed_linear_to_rpm(aux3));
     
}

void sendMotorOutput(int w1, int w2, int w3){

    //motor 1
    if (w1<0) {
        motorsOutput(PB7, PB6, -w1, 0);
    } else {
        motorsOutput(PB7, PB6, w1, 1);
    }

    //motor 2
    if (w2<0) {
        motorsOutput(PA10, PA9, -w2, 0);
    } else {
        motorsOutput(PA10, PA9, w2, 1);
    }

    //motor 3
    if (w3<0 ){
        motorsOutput(PB9, PB8, -w3, 0);
    } else {
        motorsOutput(PB9, PB8, w3, 1);
    }

}




// void controler(int rpm_target, int rpm_output){

//     float kp = 20000;
//     float kd = 0;
//     float ki = 1;


//     float erro = rpm_target - rpm_output;

//     float P = kp * erro_n;
//     float D = kd * (erro_n - erro_n_1)
//     float I = ki * (erro_n + erro_n_1)


//     e_integral = e_integral + erro*(currTime-prevTime);

    
//     float u = kp*erro + ki*e_integral;




//         int send_pwm_value = (int) fabs(u);

//         if (send_pwm_value > MAX_VALUE_MOTOR){
//             send_pwm_value=MAX_VALUE_MOTOR;
//         }

//         motorsOutput(PB8, PB9, send_pwm_value, 1);



// }