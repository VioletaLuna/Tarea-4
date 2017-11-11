//Tarea 4 métodos computacionales 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define L 0.64
#define Nx 129
#define Nt 129
#define	c 250.0
#define dt 1.0/Nt
#define N2 101
#define pi 3.14516

void condiciones_iniciales(double *x,double *phi0);
void establecer_ci(double *phi0, double** phi);
void solucionar (double** phi, double r, int n);
void escribir_datos(double *X, double ** phi, int n);

void condiciones_iniciales2(double **phi02);

int main()
{	
	//Primero es necesaario leer el archivo de condicions iniciales(ci). 
	double *x , *phi0;
	x  = malloc(Nx*sizeof(double));
	phi0 = malloc(Nx*sizeof(double));
	condiciones_iniciales(x,phi0);

	//Declaro, aparto espacio en la memoria e inicializo la matriz solución phi, con Nt, definido 
	//arbitrariamente.  
	double ** phi= malloc(Nt*sizeof(double*));
	int i;
	for ( i = 0; i < Nt; i++) 
	{
		phi[i] = malloc(Nx*sizeof(double));
	}
	establecer_ci(phi0, phi);

	//Ahora debo solucionar le acuación con la expresión que se ya se encuentra establecida. 
	double dx = x[1] - x[2];
	double r = c*(dt/dx);
	solucionar(phi,r,1);
	escribir_datos(x, phi,1);
	//Solucionamos la ecucion con la condición de frontera dada. 
	solucionar(phi,r,2);
	escribir_datos(x, phi,2);

	//Ahora vamos a hacer lo mismo pero par la ecuación diferencial en
	free(phi0);
	free(phi);
	free(x);
	double *y;
	x  = malloc(N2*sizeof(double));
	y  = malloc(N2*sizeof(double));

	double ** phi02= malloc(N2*sizeof(double*));
	for ( i = 0; i < N2; i++) 
	{
		phi02[i] = malloc(N2*sizeof(double));
	}

	double ** phi8= malloc(N2*sizeof(double*));
	double ** phi4= malloc(N2*sizeof(double*));
	double ** phi2= malloc(N2*sizeof(double*));

	condiciones_iniciales2(phi02);
	for (int i = 0; i < N2; ++i)
	{
		printf("%lf\n", phi02[0][i]);
	}

	printf("%s \n", "Fin!");
	return 0;
}

void condiciones_iniciales(double *x, double *phi0)
{
	FILE *ci;
	ci= fopen("cond_ini_cuerda.dat", "r");
	if (!ci)
	{
		printf("Problemas abriendo el archivos %s\n", "cond_ini_cuerda.dat" );
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

void establecer_ci(double *phi0, double** phi)
{
	for (int i=0; i < Nx; i++)
	{
		phi[0][i] = phi0[i];
	} 

}
//Metodo que me soluciona la ecuaión diferencial a parir de la expresión que se ha establaecido en el github.
//n es el número que representa a las condiciones de frontera, 1 ambas fijas. 
void solucionar (double** phi, double r, int n)
{
	for (int x=0; x<Nx; x++)
	{
		for (int t=0; t < Nt-1; t ++)
		{
			//Condición de extemso fijos
			if( x ==0)
			{
				phi[t+1][x]= phi[t][x];
			}
			else if (x == Nx -1)
			{
				if(n==1)
				{
					phi[t+1][x]= phi[t][x];
				}
				else 
				{
					phi[t+1][x] = sin(2*pi*c*t*dt/L);
				}
			}
			else
			{
				phi[t+1][x] = 2*(1-pow(r,2))*phi[t][x] - r*(phi[t][x+1]-phi[t][x-1]);
			}
		}
	}
}
//Metodo que escribe en un archivo los resultados de la soluciń de al ecuación. 
//n es el número que corresponde las condiciones de frontera, 1 ambas fijas.
void escribir_datos(double *X ,double** phi,int n)
{
	FILE *arch;
	if (n==1)
	{
	arch= fopen("resultados1D.dat", "w");
	}
	else
	{
		arch= fopen("resultados2D.dat", "w");
	}
	if (!arch)
	{
		printf("Problemas abriendo el archivos %s\n", "resultados.dat" );
		exit(1);
	}

	fprintf(arch, "%s\n", "x , t = 0 , t = T/8 , t = T/4 , t = T/2" );
	for (int x = 0; x < Nx; ++x)
	{
		fprintf(arch, "%e ,", X[x] );
		fprintf(arch, "%e ,", phi[0][x] );
		fprintf(arch, "%e \n", phi[15][x] );
	}	
}

void condiciones_iniciales2(double** phi0)
{
	FILE *ci;
	ci= fopen("cond_ini_tambor.dat", "r");
	if (!ci)
	{
		printf("Problemas abriendo el archivos %s\n", "cond_ini_tambor.dat" );
		exit(1);
	}
	double val;
	//En las condiocnes inicales me dan una matriz N2 x N2 donde vamos suponer que las filas son x y 
	//las columnas y.
	for (int x=0; x<N2;x++ )
	{		
		for (int y = 0; y < N2; ++y)
		{
			if (y ==0)
			{
				fscanf(ci, "%lf\n", &val);
				phi0[x][y]= val;
			}	
			fscanf(ci, "%lf", &val);
			phi0[x][y]= val;
		}
	}
	fclose(ci);
}