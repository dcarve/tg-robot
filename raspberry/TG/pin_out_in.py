M2 = 18
M1 = 23
M0 = 24

#Driver 1
DIR_1 = 4
STEP_1 = 17
SLEEP_1 = 27
RESET_1 = 22

#Driver 2
DIR_2 = 5
STEP_2 = 6
SLEEP_2 = 13
RESET_2 = 19

#Driver 3
DIR_3 = 12
STEP_3 = 16
SLEEP_3 = 20
RESET_3 = 21

def setup_pin_step_resulotion(GPIO):
    GPIO.setup(M2, GPIO.OUT)
    GPIO.setup(M1, GPIO.OUT)
    GPIO.setup(M0, GPIO.OUT)

    return GPIO


def setup_pin_driver1(GPIO):
    GPIO.setup(DIR_1, GPIO.OUT)
    GPIO.setup(STEP_1, GPIO.OUT)
    GPIO.setup(SLEEP_1, GPIO.OUT)
    GPIO.setup(RESET_1, GPIO.OUT)

    return GPIO

def setup_pin_driver2(GPIO):
    GPIO.setup(DIR_2, GPIO.OUT)
    GPIO.setup(STEP_2, GPIO.OUT)
    GPIO.setup(SLEEP_2, GPIO.OUT)
    GPIO.setup(RESET_2, GPIO.OUT)

    return GPIO

def setup_pin_driver3(GPIO):
    GPIO.setup(DIR_3, GPIO.OUT)
    GPIO.setup(STEP_3, GPIO.OUT)
    GPIO.setup(SLEEP_3, GPIO.OUT)
    GPIO.setup(RESET_3, GPIO.OUT)

    return GPIO
