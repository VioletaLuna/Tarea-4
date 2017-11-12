#Este archivo usa ecoding: utf-8

import numpy as np
import matplotlib.pyplot as plt

cuerdaFijos = np.genfromtxt("resultados1DFijo.dat", delimiter = ",", skip_header = 1)
x = cuerdaFijos[:,0]
t0f = cuerdaFijos[:,1]
t8f =cuerdaFijos[:,2]
t4f =cuerdaFijos[:,3]
t2f =cuerdaFijos[:,4]

print t8f
cuerdaPerturbacion = np.genfromtxt("resultados1DPerturbado.dat", delimiter = ",", skip_header = 1)
t0p = cuerdaPerturbacion[:,1]
t8p =cuerdaPerturbacion[:,2]
t4p =cuerdaPerturbacion[:,3]
t2p =cuerdaPerturbacion[:,4]

tambor0 = np.genfromtxt("resultados2D0.dat", delimiter = ",")

tambor8 = np.genfromtxt("resultados2D8.dat", delimiter = ",")
tambor4 = np.genfromtxt("resultados2D4.dat", delimiter = ",")
tambor2 = np.genfromtxt("resultados2D2.dat", delimiter = ",")

plt.plot(x,t0f, label = "t=0")
plt.plot(x,t8f, label = "t=T/8")
plt.plot(x,t4f, label = "t=T/4")
plt.plot(x,t2f, label = "t=T/2")
plt.legend(loc= "upper left")
plt.title("Cuerda con extremos fijos")
plt.savefig("cuerdaFijos.pdf")
plt.close()

plt.plot(x,t0p, label = "t=0")
plt.plot(x,t8p, label = "t=T/8")
plt.plot(x,t4p, label = "t=T/4")
plt.plot(x,t2p, label = "t=T/2")
plt.legend(loc= "upper left")
plt.title(u"Cuerda con un extremo con perturbación")
plt.savefig("cuerdaPerturbacion.pdf")
plt.close()

#Como para el tambor las coordenadas x, y son iguales, podemos graficar con respeccto a una variable nada más
#El movimiento en la otra componente será igual. 
