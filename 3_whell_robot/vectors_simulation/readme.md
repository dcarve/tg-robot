# Simulação dos vectores do robô de 3 rodas

## Modelo matemático


Matriz de transformação

<br>

$$\begin{pmatrix}\dot{Vw_1}\\\dot{Vw_2}\\\dot{Vw_3}\end{pmatrix} = 
\begin{pmatrix}0 & 2/3 & 1/3\\\ -1/\sqrt{3} & -1/3 & 1/3\\\ 1/\sqrt{3} & -1/3 & 1/3\end{pmatrix}   
\begin{pmatrix}\dot{V_lx}\\\ \dot{V_ly}\\\ \dot{ω}\end{pmatrix}$$



<img src="https://render.githubusercontent.com/render/math?math=\begin{pmatrix}\dot{Vw_1}\\ \dot{Vw_2}\\ \dot{Vw_3}\end{pmatrix} = ">
<img src="https://render.githubusercontent.com/render/math?math=\begin{pmatrix} 0 & 1\\ -1/\sqrt{3}  & 1\\ 1/\sqrt{3}  & 1 \end{pmatrix}">
<img src="https://render.githubusercontent.com/render/math?math=\begin{pmatrix}\dot{V_lx}\\ \dot{V_ly}\\  \dot{w}\end{pmatrix}">

<img src="https://render.githubusercontent.com/render/math?math=e^{i \pi} = -1">
