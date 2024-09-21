# -*- coding: utf-8 -*-

import math

RADIUS_ROBOT = 100  #in milimeters
DEFAULT_SPEED = 400 #mm/second
RADIUS_WHEEL = 42 #mmm - 58mm


FULL_STEP = 1
HALF_STEP = 2
QUARTER_STEP = 4
EIGHTH_STEP = 8
SIXTEENTH_STEP = 16
ONE_THIRTY_SECOND_STEP = 32

def step_resolution_encoder(step_resolution, GPIO):
    driver_mode = list(format(int(math.log2(step_resolution)), '03b'))
    driver_mode.reverse()
    driver_mode = [{'1':GPIO.HIGH,'0':GPIO.LOW}[i] for i in driver_mode]
    
    return driver_mode

def speed_linear_to_step_per_seconds(step_resolution, speed_linear):
    rpm = 60*speed_linear/(2*math.pi*RADIUS_WHEEL)

    step_per_seconds = ((rpm * step_resolution * 200) / 60)
    return int(step_per_seconds)


def transformation_matrix_rpm(linear_speed_percent, direction_angle, angular_speed, step_resolution):

    linear_speed_x = linear_speed_percent * DEFAULT_SPEED * math.cos(math.radians(direction_angle))
    linear_speed_y = linear_speed_percent * DEFAULT_SPEED * math.sin(math.radians(direction_angle))

    a11 = 0
    a12 = -2.0/3
    a13 = RADIUS_ROBOT/3
    a21 = 1/sqrt(3)
    a22 = 1.0/3
    a23 = RADIUS_ROBOT/3
    a31 = -1/sqrt(3)
    a32 = 1.0/3
    a33 = RADIUS_ROBOT/3

    speed_linear_1 = (a11 * linear_speed_x) + (a12 * linear_speed_y) + (a13 * angular_speed)
    speed_linear_2 = (a21 * linear_speed_x) + (a22 * linear_speed_y) + (a23 * angular_speed)
    speed_linear_3 = (a31 * linear_speed_x) + (a32 * linear_speed_y) + (a33 * angular_speed)
    
    w1 = speedLinearToStepPerSeconds(step_resolution, speed_linear_1)
    w2 = speedLinearToStepPerSeconds(step_resolution, speed_linear_2)
    w3 = speedLinearToStepPerSeconds(step_resolution, speed_linear_3)
     
    return w1, w2, w3


def map_logarithmic(value, in_min, in_max, out_min, out_max):
    if value<60.0:
        value=60.0

    normalized = (value - in_min) / (in_max - in_min)
    log_value = math.log10(1 + 9 * normalized)
    mapped_value = out_max + (out_min - out_max) * (1 - log_value)

    if mapped_value<0.0:
        mapped_value=0.0

    return mapped_value


def rgb_to_diretion_angle_and_magnitude(rgb):
    #baseado no algoritmo de conversão de rgb para hsl
    
    r = int(rgb[:3])/255.0
    g = int(rgb[3:6])/255.0
    b = int(rgb[6:])/255.0

    max_color = max(r, g, b)
    min_color = min(r, g, b)
    l = (max_color + min_color) / 2.0

    if max_color == min_color:
        h = 0.0  # Achromatic
    else:
        delta = max_color - min_color

        if max_color == r:
            h = (g - b) / delta + (6.0 if g < b else 0.0)
        elif max_color == g:
            h = (b - r) / delta + 2.0
        else:
            h = (r - g) / delta + 4.0

        h /= 6.0

    h = round(((360.0 - (h * 360.0)) * 100.0) / 100.0,2) # Convert to degrees and invert the x-axis
    l = round(map_logarithmic(l*100, 60.0, 90.0, 100.0, 0.0)/100,2)

    return h,l
