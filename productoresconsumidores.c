#include "omp.h"
#include <malloc.h>
#include <stdio.h>

#define N 10
#define Nthreads 5


/* Some random number constants from numerical recipies */
#define SEED       2531
#define RAND_MULT  1366
#define RAND_ADD   150889
#define RAND_MOD   714025
int randy = SEED;
int cont = 0;
double sum =0;
int semaforoC = 0;
int semaforoP = 0;
omp_lock_t lockP;




void productores(double *a);
void consumidores(double *a);
void insertar(double valor,double *a);
int extraer(double *a);

int main(){

  double *A;
  int numthreads;
   omp_init_lock(&lockP);


  	 omp_set_num_threads(Nthreads);

 	 A = (double *)malloc(N*sizeof(double));
	  
	
        numthreads = omp_get_num_threads();
        



	#pragma omp parallel shared(cont)
	{
			
			while(1){
					
				
					
					if (omp_get_thread_num() % 2 ==0)
						productores(A);
				

					else
						consumidores(A);
				

			}
	}
	omp_destroy_lock(&lockP);
}

void productores(double *a){
	
	printf("PRODUCTORES \n");

		while(1){

			


			double valor;
			//Producimos el número aleatorio
			valor = producir_num_aleatorio();
			printf("El número aleatorio es: %g\n",valor);

			//Si no hay hueco
			while(cont>=N){
				//consumidores(a);
			}
			
			//Si hay hueco lo metemos en el buffer
			insertar(valor,a);

		}
		

	
}

void insertar(double valor,double *a){

	omp_set_lock(&lockP);
		cont++;
	omp_unset_lock(&lockP);
	*(a+cont) = valor;
        printf("Valor insertado es %lf\n",*(a+cont));
	printf("contador = %d\n",cont);

}


int producir_num_aleatorio(){
	randy = (RAND_MULT * randy + RAND_ADD) % RAND_MOD;
	return randy;
}

void consumidores(double *a){

	printf("CONSUMIDORES \n");
	double valor;

	while(1){
		
		while(cont<=0){
			//productores(a);
		}

	valor = extraer(a);
	printf("El valor extraido es: %lf\n",valor);
	#pragma omp critical
		sum += valor;
	printf("La suma total es: %lf\n",sum);

	}	

}

int extraer(double *a){
	
	double valor = *(a+cont);
	
	omp_set_lock(&lockP);
		cont--;
	omp_unset_lock(&lockP);
	printf("El valor del contador es %d\n",cont);
	return valor;	

}


