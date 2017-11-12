#Makefile de la tarea 4. 
all : Plots.py

Plots.py: resultados1DFijo.dat resultados1Dperturbado.dat resultados2D0.dat resultados2D8.dat resultados2D4.dat resultados2D2.dat
	python Plots.py
