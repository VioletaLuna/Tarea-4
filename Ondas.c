//Tarea 4 métodos computacionales 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define L 0.64
#define Nx 129
#define Nt 129
#define	c 250.0
#define dt 0.01
#define dx (4.0/1000)

void coniciones_iniciales(double *x,double *phi0);

int main()
{	
	//Primero es necesaario leer el archivo de condicions iniciales(ci). 
	double *x , *phi0;
	x  = malloc(Nx*sizeof(double));
	phi0 = malloc(Nx*sizeof(double));
	coniciones_iniciales(x,phi0);

	int i;
	for (i = 0; i < Nx; ++i)
	{
		printf("%lf\n", phi0[i] );
	}

	//Declaro y aparto el espacio para mis dos variables
	double *t;
	t  = malloc(Nt*sizeof(double));
	x = malloc(Nx*sizeof(double));
	
	//Inicializo mis variables según los parametros dados. 
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

	//Escribimso los datos en un archivo

	FILE *arch;
	arch= fopen("datos.dat", "w");
	if (!arch)
	{
		printf("Problemas abriendo el archivos %s\n", "datos.dat" );
		exit(1);
	}

	fprintf(arch, "%s\n", "Función Y Timepo  posicion" );
	for (i=0;i < Nt; i++)
	{
		for ( j = 0; j < Nx; ++j)
		{
			fprintf(arch, "%f\n", y[i][j] );
			fprintf(arch, "%f", t[i] );
			fprintf(arch, "%f", x[j] );
		}
		
	}

	fclose(arch);

	printf("%s \n", "Fin!");
	return 0;
}

void coniciones_iniciales(double *x, double *phi0)
{
	FILE *ci;
	ci= fopen("cond_ini_cuerda.dat", "r");
	if (!ci)
	{
		printf("Problemas abriendo el archivos %s\n", "datos.dat" );
		exit(1);
	}
	double xval;
	double phiini;

	for (int i=0; i<Nx;i++ )
	{
		fscanf(ci, "%lf\n", &xval);
		fscanf(ci, "%lf", &phiini);
		x[i]= xval;
		phi0[i]=phiini;
	}
	fclose(ci);
}

