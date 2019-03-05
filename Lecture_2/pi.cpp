#include "mpi.h"
#include <cstdio>

using namespace std;


double sum_PI(unsigned from, unsigned to, unsigned N){
	double sum = 0.;
	for(unsigned i=from; i<=to; ++i)
		sum += 1./(1.+(i-0.5)*(i-0.5)/N/N);
	return sum/N*4.;
}

void force(double pi){return;}

int main(){
	MPI_Init(NULL, NULL);

	unsigned N = 840;

	int rank = -1;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size = -1;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	double time = MPI_Wtime();
	unsigned repeat = 10000;
	double sum, serial;
	if(rank==0){
		//SERIAL
		sum = sum_PI(1,N,N);
	
		for(unsigned i=0; i<repeat-1; ++i)
			force(sum_PI(1,N,N));

		serial = MPI_Wtime() - time;
		printf("[SERIAL]\tPI = %.20lf at rank %d, taking %lf time\n",sum,rank,serial);
	}
	MPI_Barrier(MPI_COMM_WORLD);

	//PARALLEL
	time = MPI_Wtime();

	sum = sum_PI((rank*N)/size + 1,((rank+1)*N)/size,N);
	for(unsigned i=0; i<repeat-1; ++i)
		force(sum_PI((rank*N)/size + 1,((rank+1)*N)/size,N));

	if(rank!=0)
		MPI_Ssend(&sum,1,MPI_DOUBLE,0,rank,MPI_COMM_WORLD);
	else{
		for(unsigned i=1; i<size; i++){
			double temp;
			MPI_Status status;
			MPI_Recv(&temp,1,MPI_DOUBLE,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD, &status);
			sum += temp;
		}
		double parallel = MPI_Wtime() - time;
		printf("[PARALLEL %d]\tPI = %.20lf at rank %d, taking %lf time (speedup = %lf)\n",size,sum,rank,parallel,serial/parallel);
	}

	MPI_Finalize();
}
