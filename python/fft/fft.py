# Packages and adjustments to the figures
from scipy import signal
import matplotlib.pyplot as plt
import numpy as np
import math
import pandas as pd

plt.rcParams["figure.figsize"] = 10,5
plt.rcParams["font.size"] = 16
plt.rcParams.update({"text.usetex": True,"font.family": "sans-serif","font.sans-serif": ["Helvetica"]})



df = pd.read_csv('data_readed.csv')[100:]
df['time_seconds'] = df['millis']/1000
t = df['time_seconds'].to_numpy()
y = df['rpm'].to_numpy()
samplingFreq = 100; # sampled at 1 kHz = 1000 samples / second
# Compute the Fourier transform
yhat = np.fft.fft(y);
fcycles = np.fft.fftfreq(len(t),d=1.0/samplingFreq); # the frequencies in cycles/s