#include "runMotors.h"
import encoders
import pin_out_in

#include <math.h>
#include <Arduino.h>
#include <util/atomic.h>


class motorSetup:

    def __init__(self, GPIO):
        self.GPIO = GPIO


    def step_resolution_output(self, step_resolution):
        driver_mode = encoders.step_resolution_encoder(step_resolution, self.GPIO)

        self.GPIO.output(pin_out_in.M0, driver_mode[0])
        self.GPIO.output(pin_out_in.M1, driver_mode[1])
        self.GPIO.output(pin_out_in.M2, driver_mode[2])


    def set_motor_sleep(self, motor, value):
        if motor==1:
            self.GPIO.output(pin_out_in.SLEEP_1, value)
        elif motor==2:
            self.GPIO.output(pin_out_in.SLEEP_2, value)
        elif motor==3:
            self.GPIO.output(pin_out_in.SLEEP_3, value)



    def set_motor_reset(self, motor, value):
        if motor==1:
            self.GPIO.output(pin_out_in.RESET_1, value)
        elif motor==2:
            self.GPIO.output(pin_out_in.RESET_2, value)
        elif motor==3:
            self.GPIO.output(pin_out_in.RESET_3, value)
