# -*- coding: utf-8 -*-
"""
Created on Fri Dec 23 02:09:35 2022

@author: daniel carvalho
"""


import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

path = r'D:\19_UFABC\TG\tg-robot\arduino_workspace\data\data_monitor.csv'

df = pd.read_csv(path)

df = df[4000:6000]

t = (df['millis']/1000).to_list()
s1 = df['encoderA'].to_list()
s2 = df['encoderB'].to_list()


plt.plot(t, s1)
plt.set_xlim(10, 11)
plt.grid(True)

#cxy, f = axs[1].cohere(s1, s2, 256, 1. / dt)
#axs[1].set_ylabel('Coherence')

plt.show()




import math, numpy
from matplotlib import pyplot as plt

x = numpy.linspace(0, 10, 1000)
y = numpy.array([1 if math.floor(2 * t) % 2 == 0 else 0 for t in x])

plt.plot(x,y)
plt.show()