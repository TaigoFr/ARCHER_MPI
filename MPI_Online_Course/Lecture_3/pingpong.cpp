
#include "mpi.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <cmath>
#include <iomanip> //setprecision

//file to test latency and bandwidth of communication
//to be tested using 2 CPUs and MPI

#define MPI_type MPI_CHAR
#define _type char

double pingpong(const unsigned repeat, const unsigned length){
	_type *str = (_type*)malloc(length*sizeof(_type));

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Status status;
  	int type_size;
  	MPI_Type_size(MPI_type,&type_size);

	MPI_Barrier(MPI_COMM_WORLD);
	double time = MPI_Wtime();

	for(unsigned i=0; i<repeat; ++i){
		if(rank==0){
			MPI_Ssend(&str[0], length, MPI_type, 1, 0, MPI_COMM_WORLD);
			MPI_Recv (&str[0], length, MPI_type, 1, 1, MPI_COMM_WORLD, &status);
		}
		else if(rank==1){
			MPI_Recv (&str[0], length, MPI_type, 0, 0, MPI_COMM_WORLD, &status);
			MPI_Ssend(&str[0], length, MPI_type, 0, 1, MPI_COMM_WORLD);
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	time = MPI_Wtime() - time;

	if(rank==0) printf("Size (B) =\t%u\t; time = %lf\t%e\n", length*type_size, time, time/(2.*repeat));

	free(str);
	return time;
}

int main(){
	MPI_Init(NULL,NULL);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const unsigned repeat = 1000000;
	const unsigned length = pow(2,15);
	// const double target_time = 0.5; //s

	std::ofstream file;
	if(rank==0){
		file.open("output.txt", std::ios::trunc);
		file << "Size (B)\tIterations\tTotal_time (s)\tTime (s/msg)\tBand (MB/s)" << std::endl;
	}
  	int type_size;
  	MPI_Type_size(MPI_type,&type_size);

	double ratio = 1;
	for(unsigned l=length, r=repeat; l!=0; l/=1.3, r*=ratio){
		double t = pingpong(r,l);
		double bandwidth = l * r * type_size / t * 2./1024./1024.; //each pingpong is 2 messages (1MB is 1024x1024B)
		if(rank==0) file << l << "\t\t" << (l<1000 ? "\t" : "") << r << "\t\t" << std::fixed << std::setprecision(3) << t << "\t\t\t" << std::scientific << t/(2.*r) << "\t\t" << std::fixed << bandwidth << std::endl;
		// if(r!=repeat) ratio *= target_time / t;
		// r *= target_time / t;
		// if(rank==0) printf("ratio = %lf\n",ratio);
	}

	double t = pingpong(repeat,0);
	if(rank==0) file << "FINAL " << 0 << "\t\t" << repeat << "\t\t" << std::fixed << std::setprecision(3) << t << "\t\t\t" << std::scientific << t/(2.*repeat) << "\t\t" << 0 << std::endl;

	file.close();

	MPI_Finalize();
}