# -*- coding: utf-8 -*-
"""
Created on Fri Dec 23 20:39:30 2022

@author: daniel carvalho
"""

path = r'D:\19_UFABC\TG\tg-robot\arduino_workspace\data\teste.csv'

import pandas as pd

df = pd.read_csv(path)


df['deltaT'] = (df['currT'] - df['prevT'])/1.0e6
df['velocity1'] =  (df['pos'] - df['posPrev'])/df['deltaT']

df1 = df[(df.prevT!=0)&(df.pos!=0)]

df1 = df[df.velocity1!=0]




(2860- 2830)/((13016788 - 13016775)/1.0e6)




10/0.01