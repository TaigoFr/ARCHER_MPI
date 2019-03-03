#include "mpi.h"
#include <iostream>

using namespace std;


double sum_PI(unsigned from, unsigned to, unsigned N){
	double sum = 0.;
	for(unsigned i=from; i<=to; ++i)
		sum += 1./(1.+(i-0.5)*(i-0.5)/N/N);
	return sum/N*4.;
}

int main(){
	MPI_Init(NULL, NULL);

	unsigned N = 840;

	int rank = -1;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size = -1;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//SERIAL
	double time = MPI_Wtime();
	double sum  = sum_PI(1,N,N);
	
	unsigned repeat = 100000;
	for(unsigned i=0; i<repeat-1; ++i)
		sum_PI(1,N,N);

	double serial = MPI_Wtime() - time;
	printf("[SERIAL] PI = %.10lf at rank %d, taking %lf time\n",sum,rank,serial);

	//PARALLEL
	time = MPI_Wtime();

	sum = sum_PI((rank*N)/size + 1,((rank+1)*N)/size,N);
	for(unsigned i=0; i<repeat-1; ++i)
		sum_PI((rank*N)/size + 1,((rank+1)*N)/size,N);

	if(rank!=0)
		MPI_Ssend(&sum,1,MPI_DOUBLE,0,rank,MPI_COMM_WORLD);
	else{
		for(unsigned i=1; i<size; i++){
			double temp;
			MPI_Status status;
			MPI_Recv(&temp,1,MPI_DOUBLE,i,i,MPI_COMM_WORLD, &status);
			sum += temp;
		}
		double parallel = MPI_Wtime() - time;
		printf("[PARALLEL] PI = %.10lf at rank %d, taking %lf time (speedup = %lf)\n",sum,rank,parallel,serial/parallel);
	}

	MPI_Finalize();
}
