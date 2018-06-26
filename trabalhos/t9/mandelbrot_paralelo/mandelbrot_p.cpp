#include <complex>
#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[]){
	int n_rows, start_row, last_row, max_row, max_column, max_n;
	int myrank, nproc, i;
	int args[3];
	//cin >> max_row;
	//cin >> max_column;
	//cin >> max_n;

	MPI_Init(&argc, &argv);
 	MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /* who am i */
  	MPI_Comm_size(MPI_COMM_WORLD, &nproc); /* number of processors */

	if(myrank == 0){
		cin >> args[0];
		cin >> args[1];
		cin >> args[2];
	}
	MPI_Bcast(args, 3, MPI_INT, 0, MPI_COMM_WORLD);
	max_row = args[0];
	n_rows = max_row / nproc;
	max_column = args[1];
	max_n = args[2];

	int displs[nproc];
  	int scounts[nproc];
  	int offset = 0;
  	int div_value = max_row / nproc;
 	int remainder_value = max_row % nproc;
 	for(i=0; i<nproc; i++){
    	scounts[i] = div_value * max_column;
    	if(i < remainder_value){
      		scounts[i] += max_column;
    	}
    	displs[i] = offset;
    	offset += scounts[i];
  	}

	start_row = displs[myrank] / max_column;
	last_row = start_row + scounts[myrank] / max_column;

	char mat[max_row][max_column];

	for(int r = start_row; r < last_row; ++r){
		for(int c = 0; c < max_column; ++c){
			complex<float> z;
			int n = 0;
			while(abs(z) < 2 && ++n < max_n)
				z = pow(z, 2) + decltype(z)(
					(float)c * 2 / max_column - 1.5,
					(float)r * 2 / max_row - 1
				);
			mat[r][c]=(n == max_n ? '#' : '.');
		}
	}

	MPI_Gatherv(mat[start_row], scounts[myrank], MPI_CHAR, mat, scounts, displs, MPI_CHAR, 0, MPI_COMM_WORLD);

	if(myrank == 0){
		for(int r = 0; r < max_row; ++r){
			for(int c = 0; c < max_column; ++c)
				std::cout << mat[r][c];
			cout << '\n';
		}	
	}

	MPI_Finalize();
  	return 0;
}


