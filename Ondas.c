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
#define lado 0.5

void condiciones_iniciales(double *x,double *phi0);
void establecer_ci(double *phi0, double** phi);
void solucionar (double** phi, double r, int n);
void escribir_datos(double *X, double ** phi, int n);

void condiciones_iniciales2(double **phi02);
void solucionar2(double **phi02, double **phi8, double **phi4, double **phi2);
void escribir_datos2(double **phi02, double **phi8, double **phi4, double **phi2);

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
	for ( i = 0; i < N2; i++) 
	{
		phi8[i] = malloc(N2*sizeof(double));
	}

	double ** phi4= malloc(N2*sizeof(double*));
	for ( i = 0; i < N2; i++) 
	{
		phi4[i] = malloc(N2*sizeof(double));
	}

	double ** phi2= malloc(N2*sizeof(double*));
	for ( i = 0; i < N2; i++) 
	{
		phi2[i] = malloc(N2*sizeof(double));
	}

	condiciones_iniciales2(phi02);
	solucionar2(phi02,phi8, phi4, phi2);
	escribir_datos2(phi02,phi8, phi4, phi2);

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
		arch= fopen("resultados1DFijo.dat", "w");
		if (!arch)
		{
			printf("Problemas abriendo el archivos %s\n", "resultados1DFijo.dat" );
			exit(1);
		}
	}
	else
	{
		arch= fopen("resultados1DPerturbado.dat", "w");
		if (!arch)
		{
			printf("Problemas abriendo el archivos %s\n", "resultados1DPerturbado.dat" );
			exit(1);
		}
	}
	
	fprintf(arch, "%s\n", "x , phi(t=0) , phi(t=T/8) , phi(t=T/4) , phi(t=T/2)" );
	for (int x = 0; x < Nx; ++x)
	{
		fprintf(arch, "%e , ", X[x] );
		fprintf(arch, "%e , ", phi[0][x] );
		fprintf(arch, "%e , ", phi[Nt/8][x]);
		fprintf(arch, "%e , ", phi[Nt/4][x]);
		fprintf(arch, "%e  \n", phi[Nt/2][x]);

	}
	fclose(arch);
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

void solucionar2(double **phi02, double **phi8, double **phi4, double **phi2)
{
	//Utilizando la expreción para calular el avance del tiempo del nodo central por el meotod de diferencias 
	//finitas. Tomada de: http://matematicas.uclm.es/cedya09/archive/textos/20_Urena-Prieto-F.pdf
	//Entonces debo recorrer las posiciones horizontales (x), las verticales (y) y el timepo (t).
	double v1=0;
	double v2 =0;
	double v3 =0;
	double v4 = 0;
	double v5 =0;
	double v6 =0;
	double v7=0;
	double v8=0;
	double xy0 =0;
	double antxy0 =0;
	double dt2 = 1.0/N2;
	double dxy = lado/N2;
	double ** temporal = malloc(N2*sizeof(double*));
	for ( int i = 0; i < N2; i++) 
	{
		temporal[i] = malloc(N2*sizeof(double));
	}
	
	for (int t = 0; t < N2; ++t)
	{
		for (int x = 1; x < N2-1; ++x)
		{
			for (int y = 1; y < N2-1; ++y)
			{
				if(t==0)
				{
					v1 = phi02[x-1][y-1];
					v2 = phi02[x-1][y];	
					v3 = phi02[x-1][y+1];
					v4 = phi02[x][y-1];
					v5 = phi02[x][y+1];
					v6 = phi02[x+1][y-1];
					v7 = phi02[x+1][y];
					v8 = phi02[x+1][y+1];
					xy0 = phi02[x][y];
					temporal[x][y]=2*xy0+(pow(dt,2)*pow(c,2)/(6*pow(dxy,2)))*(-20*xy0+4*v1+v2+4*v3+v4+4*v5+v6+4*v7+v8);
				}

				else
				{
					v1 = temporal[x-1][y-1];
					v2 = temporal[x-1][y];	
					v3 = temporal[x-1][y+1];
					v4 = temporal[x][y-1];
					v5 = temporal[x][y+1];
					v6 = temporal[x+1][y-1];
					v7 = temporal[x+1][y];
					v8 = temporal[x+1][y+1];
					antxy0 = xy0;
					xy0 = temporal[x][y];
					temporal[x][y]=2*xy0+ antxy0 +(pow(dt,2)*pow(c,2)/(6*pow(dxy,2)))*(-20*xy0+4*v1+v2+4*v3+v4+4*v5+v6+4*v7+v8);	
				}
			}
		}
		if (t == N2/8)
		{
			for (int x = 0; x <N2; ++x)
			{
				for (int y = 0; y <N2; ++y)
				{
					phi8[x][y]= temporal[x][y];
				}
			}
		}
		else if (t == N2/4)
		{
			for (int x = 0; x <N2; ++x)
			{
				for (int y = 0; y <N2; ++y)
				{
					phi4[x][y]= temporal[x][y];
				}
			}
		}
		else if (t == N2/2)
		{
			for (int x = 0; x <N2; ++x)
			{
				for (int y = 0; y <N2; ++y)
				{
					phi2[x][y]= temporal[x][y];
				}
			}
		}
	}
}

void escribir_datos2(double **phi02, double **phi8, double **phi4, double **phi2)
{
	FILE *arch0;
	arch0= fopen("resultados2D0.dat", "w");
	if (!arch0)
	{
		printf("Problemas abriendo el archivos %s\n", "resultados2D0.dat" );
		exit(1);
	}

	FILE *arch8;
	arch8= fopen("resultados2D8.dat", "w");
	if (!arch8)
	{
		printf("Problemas abriendo el archivos %s\n", "resultados2D8.dat" );
		exit(1);
	}

	FILE *arch4;
	arch4= fopen("resultados2D4.dat", "w");
	if (!arch4)
	{
		printf("Problemas abriendo el archivos %s\n", "resultados2D4.dat" );
		exit(1);
	}

	FILE *arch2;
	arch2= fopen("resultados2D2.dat", "w");
	if (!arch2)
	{
		printf("Problemas abriendo el archivos %s\n", "resultados2D2.dat" );
		exit(1);
	}

	//En los archivos se imprime como filas las x y como columnas las y.
	for (int x = 0; x < N2; ++x)
	{
		for (int y = 0; y < N2; ++y)
		{
			if(y==0)
			{
				fprintf(arch0, "%lf", phi02[x][y]);
				fprintf(arch8, "%lf", phi8[x][y]);
				fprintf(arch4, "%lf", phi4[x][y]);
				fprintf(arch2, "%lf", phi2[x][y]);
			}
			else if (y==N2-1)
			{
				fprintf(arch0, "%lf \n", phi02[x][y]);
				fprintf(arch8, "%lf \n", phi8[x][y]);
				fprintf(arch4, "%lf \n", phi4[x][y]);
				fprintf(arch2, "%lf \n", phi2[x][y]);
			}
			else 
			{
				fprintf(arch0, "%lf , ", phi02[x][y]);
				fprintf(arch8, "%lf , ", phi8[x][y]);
				fprintf(arch4, "%lf , ", phi4[x][y]);
				fprintf(arch2, "%lf , ", phi2[x][y]);
			}
			
		}
		
	}

	fclose(arch0);
	fclose(arch8);
	fclose(arch4);
	fclose(arch2);
}