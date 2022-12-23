#include "pwm.h"

void set_pwm_value(byte pin, int value){
    pwmWrite(pin, value);
}
