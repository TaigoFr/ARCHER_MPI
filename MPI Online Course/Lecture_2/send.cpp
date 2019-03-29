#include "mpi.h"
#include <iostream>

using namespace std;

int main(){
	MPI_Init(NULL, NULL);

	int rank = -1;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size = -1;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// cout << "Hello World from rank " << rank << " (pool size is " << size << ")" << endl;

	int x[3] = {100,200,300};
	unsigned from = 1;
	unsigned to   = 3;
	if(rank==from){
		MPI_Ssend(x,2,MPI_INT,to,0,MPI_COMM_WORLD);
	}
	x[0] = 0;
	x[1] = 1;
	x[2] = 2;
	MPI_Status status;
	if(rank==3){
		MPI_Recv(x,3,MPI_INT,1,0,MPI_COMM_WORLD, &status);
	}

	cout << "x = " << x[0] << "," << x[1] << "," << x[2] << " in rank " << rank << endl;

	int count = -1;
	MPI_Get_count(&status, MPI_INT, &count);
	cout << "status = " << status.MPI_SOURCE << "," << status.MPI_TAG << "," << count << " in rank " << rank << endl;

	MPI_Finalize();
}
