#Makefile de la tarea 4. 
all: cuerdaFijos.pdf

cuerdaFijos.pdf: Plots.py
	python Plots.py &

Plots.py: Ondas.c 
	cc Ondas.c -o Ondas.x -lm
	./Ondas.x 	
	python Plots.py &
