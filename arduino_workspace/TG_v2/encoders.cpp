#include "encoders.h"
#include <math.h>
#include <Arduino.h>
#include <util/atomic.h>


void stepResolutionEncoder(int stepResolution, byte* driver_mode){

    switch (stepResolution) {
        case 0:
            driver_mode[0] = LOW;
            driver_mode[1] = LOW;
            driver_mode[2] = LOW;
            break;
        case 1:
            driver_mode[0] = HIGH;
            driver_mode[1] = LOW;
            driver_mode[2] = LOW;
            break;
        case 2:
            driver_mode[0] = LOW;
            driver_mode[1] = HIGH;
            driver_mode[2] = LOW;
            break;
        case 3:
            driver_mode[0] = HIGH;
            driver_mode[1] = HIGH;
            driver_mode[2] = LOW;
            break;
        case 4:
            driver_mode[0] = LOW;
            driver_mode[1] = LOW;
            driver_mode[2] = HIGH;
            break;
        case 5:
            driver_mode[0] = HIGH;
            driver_mode[1] = LOW;
            driver_mode[2] = HIGH;
            break;  
    }
}

