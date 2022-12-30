# -*- coding: utf-8 -*-
"""
Created on Sat Dec 24 12:24:54 2022

@author: daniel carvalho
"""

t1 = {"V":4.66, "RPM": 178.19, "bytes": 65535}
t1 = {"V":4.59, "RPM": 174.70, "bytes": 65000}
t2 = {"V":2.02, "RPM": 75.14, "bytes": 20000}
t2 = {"V":3.26, "RPM": 124.08, "bytes": 40000}
t2 = {"V":0.90, "RPM": 27.71, "bytes": 10000}
t2 = {"V":0.49, "RPM": 12.23, "bytes": 8000}
 

def calc_rpm(x):
    return -6.892 + 39.786*x


def calc_bytes(x):
    return -15991.485 + 17525.274*x


projecao_V = 5.5
projecao_RPM =   calc_rpm(projecao_V)
projecao_bytes = calc_bytes(projecao_V)

          
          
          
          
          
         


