#include "runMotors.h"
import encoders
#include "pinOutIn.h"

#include <math.h>
#include <Arduino.h>
#include <util/atomic.h>


class motors


    def __init__(self, GPIO):
        self.GPIO = GPIO


def stepResolution(step_resolution, GPIO):
    encoders.step_resolution_encoder(step_resolution)

    digitalWrite(M0, driverMode[0]);
    digitalWrite(M1, driverMode[1]);
    digitalWrite(M2, driverMode[2]);
}

/* 
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
 */

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

void SetMotorReset(int motor, byte value){

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