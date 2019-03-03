
//TODO's
// protection agains wrong ranks sent to send or receive
// add wildcards any source and any tag (saved in status)
// maximum tag is 32767
// Wtime
// MPI_Probe

#pragma once

#include "mpi.h"

#include <string>

class MPICPP{
public:
	explicit MPICPP();
	~MPICPP();

	int rank();
	int size();
	std::string name();

private:
	void error(std::string method, std::string MPI_method);

	MPI_Comm comm;
};