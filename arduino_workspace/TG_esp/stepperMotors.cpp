#include "stepperMotors.h"
#include "encoders.h"
#include "pinOutIn.h"

#include <math.h>
#include <Arduino.h>


void SetMotorSleep(int motor, byte value){

    switch (motor) {
        case 1:
            digitalWrite(SLEEP_1, value);
            break;
        case 2:
            digitalWrite(SLEEP_2, value);
            break;
        case 3:
            digitalWrite(SLEEP_3, value);
            break; 
    }

}

void SetMotorResetAll(byte value){
    digitalWrite(RESET_ALL, value);
}