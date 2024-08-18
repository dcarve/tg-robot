#include "runMotors.h"
#include "encoders.h"
#include <math.h>
#include <Arduino.h>
#include <util/atomic.h>
#include "pinOutIn.h"

// pins A11, A12, A13, B3, B4

byte driverMode[] = {LOW, LOW, LOW};

void stepResolution(int stepResolution){
    stepResolutionEncoder(stepResolution, driverMode);

    digitalWrite(M0, driverMode[0]);
    digitalWrite(M1, driverMode[1]);
    digitalWrite(M2, driverMode[2]);
}

