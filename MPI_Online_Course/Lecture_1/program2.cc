#include "mpi.h"
#include <iostream>

using namespace std;

int main(){
	MPI_Init(NULL, NULL);

	int rank = -1;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size = -1;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	cout << "Hello World from rank " << rank << " (pool size is " << size << ")" << endl;

	int namelen;
	char procname[MPI_MAX_PROCESSOR_NAME];
	
	MPI_Get_processor_name(procname, &namelen);
	cout << "rank " << rank << " is on machine " << procname << endl;

	MPI_Finalize();
}
