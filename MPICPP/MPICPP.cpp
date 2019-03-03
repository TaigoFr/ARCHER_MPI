
#include "MPICPP.hpp"

#include  <stdexcept> //std::runtime_error
#include  <iostream>

MPICPP::MPICPP(){
	MPI_Init(NULL, NULL);
	comm = MPI_COMM_WORLD;
}

MPICPP::~MPICPP(){
	MPI_Finalize();
}

int MPICPP::rank(){
	int rank = -1;
	int err = MPI_Comm_rank(comm, &rank);
	if(err != MPI_SUCCESS)
		error("rank","MPI_Comm_rank");

	return rank;
}

int MPICPP::size(){
	int size = -1;
	int err = MPI_Comm_size(comm, &size);
	if(err != MPI_SUCCESS)
		error("size","MPI_Comm_size");

	return size;
}

std::string MPICPP::name(){
	int namelen;
	char procname[MPI_MAX_PROCESSOR_NAME];
	int err = MPI_Get_processor_name(procname, &namelen);
	if(err != MPI_SUCCESS)
		error("name","MPI_Get_processor_name");

	return std::string(procname,namelen);
}


void MPICPP::error(std::string method, std::string MPI_method){
	throw std::runtime_error(("Error in method " + method + " when calling ",MPI_method).c_str());
}
