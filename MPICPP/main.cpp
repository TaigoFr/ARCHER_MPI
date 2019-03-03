
#include "MPICPP.hpp"

#include <iostream>

int main(){
	MPICPP mpi;

	std::cout << "Hello World from rank " << mpi.rank() << ", in machine " << mpi.name() << " (pool of size " << mpi.size() << ")" << std::endl;
	
	return 0;
}