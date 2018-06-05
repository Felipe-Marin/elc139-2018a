#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

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

int main(int argc, char* argv[]) {
    //mpi
    int myrank;         // "rank" do processo (0 a P-1)
    int p;              // n�mero de processos
    int source;         // "rank" do processo remetente
    int dest;           // "rank" do processo destinat�rio
    int tag = 0;        // "etiqueta" da mensagem
    double msg[1];      // a mensagem
    MPI_Status status;  // "status" de uma opera��o efetuada

    // dotprod
    int wsize, repeat;
    double sum;
    double start_time, end_time;

    //parametros
    wsize = atoi(argv[1]);  // worksize = tamanho do vetor de cada thread
    repeat = atoi(argv[2]); // numero de repeticoes dos calculos (para aumentar carga)

    // MPI_Init deve ser invocado antes de qualquer outra chamada MPI
    MPI_Init(&argc, &argv);
    // Descobre o "rank" do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    // Descobre o número de processos
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    //tempo no inicio da execução
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    if (myrank != 0) {
        dotdata.a = (double *) malloc(wsize*sizeof(double));
        fill(dotdata.a, wsize, 0.01);
        dotdata.b = (double *) malloc(wsize*sizeof(double));
        fill(dotdata.b, wsize, 1.0);
        dotdata.wsize = wsize;
        dotdata.repeat = repeat;
        msg[0] = dotprod(dotdata);
        dest = 0;
        MPI_Send(msg, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
    } else {
        sum = 0.0;
        for(source = 1; source < p; source++) {
            MPI_Recv(msg, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
            sum += msg[0];
        }
    }

    //tempo no fim da execução
    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    MPI_Finalize(); // finaliza MPI

    if(myrank == 0){
        printf("Dotprod: %f\n", sum);
        printf("Runtime: %f sec\n", (end_time - start_time));  
    }

    return 0;
}