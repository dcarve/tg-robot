# -*- coding: utf-8 -*-
"""
Created on Mon Sep 18 14:01:00 2023

@author: daniel carvalho
"""

import json
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

res = open('mediadas_capturadas.txt').read()
res = res.split('\n')

res = [json.loads(i[i.find('''{"millis":'''):]) for i in res]

dados_brutos = pd.DataFrame(res)

dados_brutos = pd.concat([
    dados_brutos[['w1','filterRpm_1']]
    .rename(columns={'filterRpm_1':'filter_rpm_','w1':'w'}),
    dados_brutos[['w2','filterRpm_2']]
    .rename(columns={'filterRpm_2':'filter_rpm_','w2':'w'}),
    dados_brutos[['w3','filterRpm_3']]
    .rename(columns={'filterRpm_3':'filter_rpm_','w3':'w'})
]).sort_values('w')


dados_medios = dados_brutos.groupby('w').agg({
    'filter_rpm_':['mean']
}).reset_index()


dados_medios.columns = [''.join(i) for i in dados_medios.columns]

dados_medios['cut'] = (dados_medios['w']/200).astype(int)
dados_medios_amostra = dados_medios.groupby('cut').first().reset_index()


dados_medios_amostra[['filter_rpm_mean', 'w']].to_csv('amostra_invertida.csv', index=False)




#fit poly3 com matlab


#func_string = "a*x**3 + b*x**2 + c*x + d"
func_string = "a*x**4 + b*x**3 + c*x**2 + d*x + e"

def func(x, a, b, c, d, e):
    return  eval(func_string)

#a,b,c,d = [0.03571,-11.16,1292,-3.396e+04]

#a,b,c,d = [0.02138,-4.968,437,3350]

a,b,c,d,e = [0.0001131, -0.03064, 2.993, -1.257, 9017]


x = np.array(range(0,65535))
y = func(
    x=x,
    a=a,
    b=b,
    c=c,
    d=d,
    e=e,
   )


my_dpi=100
fig1 = plt.figure(figsize=(600/my_dpi, 500/my_dpi), dpi=my_dpi)
fig2 = plt.figure(figsize=(600/my_dpi, 500/my_dpi), dpi=my_dpi)
fig3 = plt.figure(figsize=(600/my_dpi, 500/my_dpi), dpi=my_dpi)

ax11 = fig1.add_subplot(1, 1, 1)
ax12 = fig2.add_subplot(1, 1, 1)
ax13 = fig3.add_subplot(1, 1, 1)


ax11.set_title("Curva PWM x RPM dados brutos")
ax11.set_ylabel("PWM")
ax11.set_xlabel("RPM")
ax11.set_xlim([0, 210])
ax11.set_ylim([0, 70000])


ax12.set_title("Curva PWM x RPM dados médios")
ax12.set_ylabel("PWM")
ax12.set_xlabel("RPM")
ax12.set_xlim([0, 210])
ax12.set_ylim([0, 70000])

ax13.set_title("Curva ajustada")
ax13.set_ylabel("PWM")
ax13.set_xlabel("RPM")
ax13.set_xlim([0, 210])
ax13.set_ylim([0, 70000])


ax11.plot(
    dados_brutos['filter_rpm_'],  dados_brutos['w'], '.' ,color="#3684eb", label="rpm 1", markersize=0.5
)

ax12.plot(
    dados_medios['filter_rpm_mean'],  dados_medios['w'], '.' ,color="#3684eb", label="rpm 1", markersize=0.5
)


ax13.plot(
    dados_medios['filter_rpm_mean'],  dados_medios['w'], '.' ,color="#3684eb", label="rpm 1", markersize=0.5
)


ax13.plot(
   x,  y,color="red", label="curva medida", linewidth=2
)


func_string = (
    func_string
    .replace('a', str(a))
    .replace('b', str(b))
    .replace('c', str(c))
    .replace('d', str(d))
    .replace('e', str(e))
)

ax13.text(0.95, 0.01, f'{func_string}',
       verticalalignment='bottom', horizontalalignment='right',
        transform=ax13.transAxes,
        color='red', fontsize=8)
