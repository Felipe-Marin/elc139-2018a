#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct 
 {
   double *a;
   double *b;
   int wsize;
   int repeat; 
 } dotdata_t;

dotdata_t dotdata;

/*
 * Funcao executada pelos processos
 */
double dotprod()
{
    int i, k;
    double *a = dotdata.a;
    double *b = dotdata.b;     
    int wsize = dotdata.wsize;
    double mysum;

    for (k = 0; k < dotdata.repeat; k++) {
        mysum = 0.0;
        for (i = 0; i < wsize ; i++)  {
            mysum += (a[i] * b[i]);
        }
    }
    return mysum;
}

/*
 * Preenche vetor
 */ 
void fill(double *a, int size, double value)
{  
   int i;
   for (i = 0; i < size; i++) {
      a[i] = value;
   }
}

/*
 * Tempo (wallclock) em microssegundos
 */ 
long wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

int main(int argc, char* argv[]) {
    // dotprod
    int wsize, repeat;
    double sum;
    long start_time, end_time;

    //parametros
    wsize = atoi(argv[1]);  // worksize = tamanho do vetor
    repeat = atoi(argv[2]); // numero de repeticoes dos calculos (para aumentar carga)

    start_time = wtime();

    dotdata.a = (double *) malloc(wsize*sizeof(double));
    fill(dotdata.a, wsize, 0.01);
    dotdata.b = (double *) malloc(wsize*sizeof(double));
    fill(dotdata.b, wsize, 1.0);
    dotdata.wsize = wsize;
    dotdata.repeat = repeat;
    sum = dotprod();

    //tempo no fim da execução
    end_time = wtime();

    printf("Dotprod: %f\n", sum);
    printf("Runtime: %ld sec\n", (end_time - start_time)); 

    return 0;
}