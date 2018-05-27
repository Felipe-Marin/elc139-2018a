#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
    int myrank;         // "rank" do processo (0 a P-1)
    int p;              // número de processos
    int source;         // "rank" do processo remetente
    int dest;           // "rank" do processo destinatário
    int tag = 0;        // "etiqueta" da mensagem
    int msg[0];      // a mensagem
    MPI_Status status;  // "status" de uma operação efetuada

    // MPI_Init deve ser invocado antes de qualquer outra chamada MPI
    MPI_Init(&argc, &argv);
    // Descobre o "rank" do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    // Descobre o número de processos
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (myrank > 0) {
        // recebe a mensagem do processo anterior
        source = myrank - 1;
        MPI_Recv(msg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
        msg[0] += 1;
        // envia mensagem ao próximo processo
        if(myrank < p - 1){
            dest = myrank + 1;
            MPI_Send(msg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        }
    } else {
        msg[0] = 0;
        dest = myrank +1;
        if(p > 1){
            MPI_Send(msg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize(); // finaliza MPI

    //se for o ultimo processo mostra a mensagem
    if(myrank == p - 1){
        printf("Mensagem: %d\n", msg[0]);
    }

    return 0;
}
