
#include <mpi.h>
#include <stdio.h>

#define SIZE 8      /* Size of matrices */

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];


void fill_matrix(int m[SIZE][SIZE])
{
  static int n=0;
  int i, j;
  for (i=0; i<SIZE; i++)
    for (j=0; j<SIZE; j++)
      m[i][j] = n++;
}

void print_matrix(int m[SIZE][SIZE])
{
  int i, j = 0;
  for (i=0; i<SIZE; i++) {
    printf("\n\t| ");
    for (j=0; j<SIZE; j++)
      printf("%*d", 6, m[i][j]);
    printf("|");
  }
}

int main(int argc, char *argv[])
{
  int myrank, nproc, from, to, h, i, j, k;
  int tag_A = 0;
  int tag_B = 1;
  MPI_Status status;
  
  MPI_Init (&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /* who am i */
  MPI_Comm_size(MPI_COMM_WORLD, &nproc); /* number of processors */

  /*if (SIZE%nproc!=0) {
    if (myrank==0) printf("Matrix size not divisible by number of processors\n");
    MPI_Finalize();
    return -1;
  }*/

  /* Process 0 fills the input matrices and broadcasts them to the rest */
  /* (actually, only the relevant stripe of A is sent to each process) */

  if (myrank==0) {
    fill_matrix(A);
    fill_matrix(B);
  }

  int displs[nproc];
  int scounts[nproc];
  int offset = 0;
  int div_value = SIZE / nproc;
  int remainder_value = SIZE % nproc;
  for(i=0; i<nproc; i++){
    scounts[i] = div_value * SIZE;
    if(i < remainder_value){
      scounts[i] += SIZE;
    }
    displs[i] = offset;
    offset += scounts[i];
  }

  int recvbuf[scounts[myrank]/SIZE][SIZE];

  MPI_Bcast(B, SIZE*SIZE, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatterv(A, scounts, displs, MPI_INT, recvbuf, scounts[myrank], MPI_INT, 0, MPI_COMM_WORLD);

  from = displs[myrank] / SIZE;
  to = from + scounts[myrank] / SIZE;

  printf("computing slice %d (from row %d to %d)\n", myrank, from, to-1);
  for (i=from, h=0; i<to; i++, h++) {
    for (j=0; j<SIZE; j++) {
      C[i][j]=0;
      for (k=0; k<SIZE; k++){
        C[i][j] += recvbuf[h][k]*B[k][j];
      }
    }
  }

  MPI_Gatherv(C[from], scounts[myrank], MPI_INT, C, scounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

  if (myrank==0) {
    printf("\n\n");
    print_matrix(A);
    printf("\n\n\t       * \n");
    print_matrix(B);
    printf("\n\n\t       = \n");
    print_matrix(C);
    printf("\n\n");
  }

  MPI_Finalize();
  return 0;
}



