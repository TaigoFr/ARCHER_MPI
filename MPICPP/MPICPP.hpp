
//TODO's
// protection agains wrong ranks sent to send or receive
// add wildcards any source and any tag (saved in status)
// maximum tag is 32767
// Wtime
// MPI_Probe
// MPI_Barrier

#ifndef MPICPP_
#define MPICPP_

#include "mpi.h"

#include <string>

#define Mif(mpi,rk)  if(mpi.rank() == rk)
#define Mifn(mpi,rk) if(mpi.rank() != rk)

class MPICPP{
public:
	explicit MPICPP();
	~MPICPP();

	int rank();
	int size();
	std::string name();

	template <typename T>
	void send(T &t, int to, int from); // from == -1 for any source

	double barry();
	double time();

private:
	void error(std::string method, std::string MPI_method);

	template <typename T>
	void send_type(const T&, int to, int tag);
	template <typename T>
	void recv_type(T&, int from, int tag);


	MPI_Comm comm;
	MPI_Status status;
	int m_rank, m_size;
};

#include "MPICPP.impl.hpp"

#endif /* MPICPP_ */