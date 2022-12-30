from scipy import signal
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
    return filter(data)

def low_pass_filter_rpm_2(data):
    return filter(data)


def filter(y):
    freq = 10
    w0 = (2*np.pi)*freq; # pole frequency (rad/s)
    num = w0        # transfer function numerator coefficients
    den = [1,w0]    # transfer function denominator coefficients
    lowPass = signal.TransferFunction(num,den) # Transfer function
    dt = 1.0/SAMPLING_FREQ
    discreteLowPass = lowPass.to_discrete(dt,method='gbt',alpha=0.5)

    b = discreteLowPass.num
    a = -discreteLowPass.den

    # Filter the signal
    yfilt = np.zeros(len(y));
    for i in range(3,len(y)):
        yfilt[i] = a[1]*yfilt[i-1] + b[0]*y[i] + b[1]*y[i-1]

    return yfilt