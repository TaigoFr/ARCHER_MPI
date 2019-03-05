
#include "MPICPP.hpp"

#include <iostream>

int main(){
	MPICPP mpi;

	// printf("Hello World from rank %d, in machine %s (pool of size %d)\n", mpi.rank(), mpi.name().c_str(), mpi.size());
	int i = mpi.rank();
	mpi.send(i,1,3);
	// mpi.send(double(),1,1);

	printf("i = %d (rank %d)\n",i,mpi.rank());

	return 0;
}