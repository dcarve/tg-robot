#include "runMotors.h"
#include "encoders.h"
#include "pinOutIn.h"

#include <math.h>
#include <Arduino.h>
#include <util/atomic.h>

// pins A11, A12, A13, B3, B4

byte driverMode[] = {LOW, LOW, LOW};

void stepResolution(int stepResolution){
    stepResolutionEncoder(stepResolution, driverMode);

    digitalWrite(M0, driverMode[0]);
    digitalWrite(M1, driverMode[1]);
    digitalWrite(M2, driverMode[2]);
}

void convertRpmToDelays(
    int *w1_delay,
    int *w2_delay,
    int *w3_delay,
    float w1,
    float w2,
    float w3,
    int stepResolution)
{

    *w1_delay = delayMicrosecondsForStep(stepResolution, w1);
    *w2_delay = delayMicrosecondsForStep(stepResolution, w2);
    *w3_delay = delayMicrosecondsForStep(stepResolution, w3);

}


void SetSleep(int motor, byte value){

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

void SetReset(int motor, byte value){

    switch (motor) {
        case 1:
            digitalWrite(RESET_1, value);
            break;
        case 2:
            digitalWrite(RESET_2, value);
            break;
        case 3:
            digitalWrite(RESET_3, value);
            break; 
    }

}

void SetStep(int motor, byte value){

    switch (motor) {
        case 1:
            digitalWrite(STEP_1, value);
            break;
        case 2:
            digitalWrite(STEP_2, value);
            break;
        case 3:
            digitalWrite(STEP_3, value);
            break; 
    }

}
