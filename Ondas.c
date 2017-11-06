//Tarea 4 métodos computacionales 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define Nt 700
#define Nx 1000
#define	c 1.0
#define dt 0.01
#define dx (4.0/1000)

//Primero śolucionamos la ecuación de adevección que es una sola dimensión. Que además es el mismo del taller
// calificable del laboratorio. 

int main()
{	
	//Declaro y aparto el espacio para mis dos variables
	double *t , *x;
	t  = malloc(Nt*sizeof(double));
	x = malloc(Nx*sizeof(double));
	
	//Inicializo mis variables según los parametros dados. 
	int i;
	t[0]= 0.0;
	for ( i = 1; i < Nt; i++)
	{
		t[i] = t[i-1] + dt;
	} 

	x[0]=-2.0;
	for ( i = 1; i < Nx; i++) 
	{
		x[i] = x[i-1] + dx;
	}

	//Declaro, aparto espacio enla memoria e inicializo la matriz solución y.  
	double ** y= malloc(Nt*sizeof(double*));
	for ( i = 0; i < Nt; i++) 
	{
		y[i] = malloc(Nx*sizeof(double));
	}

	//Establecemos la condicón inicial
	for (i=0; i < Nx; i++)
	{
		y[0][i] = exp(-100*(pow(x[i],2)));

	} 

	int j;
	for (j=0; j<Nx; j++)
	{
		for (i=0; i < Nt-1; i ++)
		{
			//Condición de extemso fijos
			if(j ==0 || j == Nx -1)
			{
				y[i+1][j]= y[i][j];

			}
			else
			{
				y[i+1][j] = y[i][j] + c*(dt/dx)*(y[i][j]-y[i][j-1]);
			}
			
		}
	}



	printf("%s \n", "Fin!");
	return 0;
}

