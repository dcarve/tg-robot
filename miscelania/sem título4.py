# -*- coding: utf-8 -*-
"""
Created on Mon Sep 18 14:01:00 2023

@author: daniel carvalho
"""

import json
import pandas as pd

import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.ticker as ticker
import json



result = open(r'D:\19_UFABC\TG\tg-robot\results.txt').read()

result = result.split('\n')

result = [json.loads(line[line.find('''{"millis"'''):]) for line in result]

df = pd.DataFrame(result)

rpm1_list = df['rpm1']
rpm2_list = df['rpm2']
rpm3_list = df['rpm3']

filter_rpm_1_list = df['filterRpm_1']
filter_rpm_2_list = df['filterRpm_2']
filter_rpm_3_list = df['filterRpm_3']

w1_list = df['w1']
w2_list = df['w2']
w3_list = df['w3']

seconds_list = df['millis']


my_dpi=100
fig = plt.figure(figsize=(2626/my_dpi, 911/my_dpi), dpi=my_dpi)



ax11 = fig.add_subplot(2, 3, 1)
ax12 = fig.add_subplot(2, 3, 2)
ax13 = fig.add_subplot(2, 3, 3)

ax21 = fig.add_subplot(2, 3, 4)
ax21_2 = ax21.twinx()

ax22 = fig.add_subplot(2, 3, 5)
ax22_2 = ax22.twinx()

ax23 = fig.add_subplot(2, 3, 6)
ax23_2 = ax23.twinx()



ax11.set_ylim([0, 220])
ax11.set_title("rpm 1")
ax11.set_xlabel("secconds")
ax11.set_ylabel("rpm")

ax12.set_ylim([0, 220])
ax12.set_title("rpm 2")
ax12.set_xlabel("secconds")
ax12.set_ylabel("rpm")

ax13.set_ylim([0, 220])
ax13.set_title("rpm 3")
ax13.set_xlabel("secconds")
ax13.set_ylabel("rpm")


ax21.set_ylim([0, 220])
ax21.set_title("RPM x PWM 1")
ax21.set_xlabel("secconds")
ax21.set_ylabel("rpm")

ax21_2.set_ylim([0, 65540])
ax21_2.set_ylabel("pwm")

ax22.set_ylim([0, 220])
ax22.set_title("RPM x PWM 2")
ax22.set_xlabel("secconds")
ax22.set_ylabel("rpm")

ax22_2.set_ylim([0, 65540])
ax22_2.set_ylabel("pwm")

ax23.set_ylim([0, 220])
ax23.set_title("RPM x PWM 3")
ax23.set_xlabel("secconds")
ax23.set_ylabel("rpm")

ax23_2.set_ylim([0, 65540])
ax23_2.set_ylabel("pwm")

ax11.plot(
    seconds_list, rpm1_list, color="#3684eb", label="rpm 1"
)
ax21.plot(
    seconds_list, filter_rpm_1_list, color="#3684eb", label="rpm filtered 1"
)
ax21_2.plot(
    seconds_list, w1_list, color="#ff9d00", label="pwm 1"
)


ax12.plot(
    seconds_list, rpm2_list, color="#3684eb", label="rpm 2"
)
ax22.plot(
    seconds_list, filter_rpm_2_list, color="#3684eb", label="rpm filtered 2"
)
ax22_2.plot(
    seconds_list, w2_list, color="#ff9d00", label="pwm 2"
)


ax13.plot(
    seconds_list, rpm3_list, color="#3684eb", label="rpm 3"
)
ax23.plot(
    seconds_list, filter_rpm_3_list, color="#3684eb", label="rpm filtered 3"
)
ax23_2.plot(
    seconds_list, w3_list, color="#ff9d00", label="pwm 3"
)


ax11.legend()
ax21.legend()
ax21_2.legend()

ax12.legend()
ax22.legend()
ax22_2.legend()

ax13.legend()
ax23.legend()
ax23_2.legend()




