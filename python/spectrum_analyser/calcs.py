
import numpy as np

SAMPLING_FREQ = 100

def fft_values(y):
    res =  np.fft.fft(y)
    return np.absolute(res)

def fcycles(t):
    return np.fft.fftfreq(len(t),d=1.0/SAMPLING_FREQ)

def millis_to_seconds(millis):
    return millis/1000


def low_pass_filter_rpm_1(data):
    return data

def low_pass_filter_rpm_2(data):
    return data