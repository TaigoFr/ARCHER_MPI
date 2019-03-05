
#if !defined(MPICPP_)
#error "This file should only be included through DatabaseWriter.h"
#endif

#include  <stdexcept> //std::runtime_error
// #include  <iostream>

MPICPP::MPICPP(): m_rank(-1), m_size(-1){
	MPI_Init(NULL, NULL);
	comm = MPI_COMM_WORLD;
}

MPICPP::~MPICPP(){
	MPI_Finalize();
}

int MPICPP::rank(){
	if(m_rank >= 0) return m_rank;

	int err = MPI_Comm_rank(comm, &m_rank);
	if(err != MPI_SUCCESS)
		error("rank","MPI_Comm_rank");

	return m_rank;
}

int MPICPP::size(){
	if(m_size >= 0) return m_size;

	int err = MPI_Comm_size(comm, &m_size);
	if(err != MPI_SUCCESS)
		error("size","MPI_Comm_size");

	return m_size;
}

std::string MPICPP::name(){
	int namelen;
	char procname[MPI_MAX_PROCESSOR_NAME];
	int err = MPI_Get_processor_name(procname, &namelen);
	if(err != MPI_SUCCESS)
		error("name","MPI_Get_processor_name");

	return std::string(procname,namelen);
}


double MPICPP::barry(){ return MPI_Barrier(comm); 	}
double MPICPP::time() { return MPI_Wtime(); 		}


void MPICPP::error(std::string method, std::string MPI_method){
	throw std::runtime_error(("Error in method " + method + " when calling ",MPI_method).c_str());
}

template <typename T>
void MPICPP::send(T &t, int to, int from){
	if(to == rank())
		recv_type(t,from,from);
	else if(from == rank())
		send_type(t,to,from);
}

template<> void MPICPP::send_type<int>(const int& t, int to, int tag)
{ MPI_Ssend(&t, 1, MPI_INT, to, tag, comm); }
template<> void MPICPP::recv_type<int>(int& t, int from, int tag)
{ MPI_Recv (&t, 1, MPI_INT, from, tag, comm, &status); }

template<> void MPICPP::send_type<double>(const double& t, int to, int tag)
{ MPI_Ssend(&t, 1, MPI_DOUBLE, to, tag, comm); }
template<> void MPICPP::recv_type<double>(double& t, int from, int tag)
{ MPI_Recv (&t, 1, MPI_DOUBLE, from, tag, comm, &status); }

template<> void MPICPP::send_type<char>(const char& t, int to, int tag)
{ MPI_Ssend(&t, 1, MPI_CHAR, to, tag, comm); }
template<> void MPICPP::recv_type<char>(char& t, int from, int tag)
{ MPI_Recv (&t, 1, MPI_CHAR, from, tag, comm, &status); }

template<> void MPICPP::send_type<float>(const float& t, int to, int tag)
{ MPI_Ssend(&t, 1, MPI_FLOAT, to, tag, comm); }
template<> void MPICPP::recv_type<float>(float& t, int from, int tag)
{ MPI_Recv (&t, 1, MPI_FLOAT, from, tag, comm, &status); }

template<> void MPICPP::send_type<unsigned>(const unsigned& t, int to, int tag)
{ MPI_Ssend(&t, 1, MPI_UNSIGNED, to, tag, comm); }
template<> void MPICPP::recv_type<unsigned>(unsigned& t, int from, int tag)
{ MPI_Recv (&t, 1, MPI_UNSIGNED, from, tag, comm, &status); }

