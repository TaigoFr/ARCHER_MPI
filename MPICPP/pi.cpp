#include "MPICPP.hpp"

#include <iostream>

using namespace std;


double sum_PI(unsigned from, unsigned to, unsigned N){
	double sum = 0.;
	for(unsigned i=from; i<=to; ++i)
		sum += 1./(1.+(i-0.5)*(i-0.5)/N/N);
	return sum/N*4.;
}

int main(){
	MPICPP mpi;

	unsigned N = 840;

	double time = mpi.time();
	unsigned repeat = 10000;
	double sum, serial;
	Mif(mpi,0){
		//SERIAL
		sum = sum_PI(1,N,N);
	
		for(unsigned i=0; i<repeat-1; ++i)
			sum_PI(1,N,N);

		serial = mpi.time() - time;
		printf("[SERIAL]\tPI = %.20lf at rank %d, taking %lf time\n",sum,mpi.rank(),serial);
	}
	MPI_Barrier(MPI_COMM_WORLD);

	//PARALLEL
	time = mpi.time();

	sum = sum_PI((mpi.rank()*N)/mpi.size() + 1,((mpi.rank()+1)*N)/mpi.size(),N);
	for(unsigned i=0; i<repeat-1; ++i)
		sum_PI((mpi.rank()*N)/mpi.size() + 1,((mpi.rank()+1)*N)/mpi.size(),N);

	for(unsigned i=1; i<mpi.size(); i++){
		double temp = sum;
		mpi.send(temp,0,i);
		Mif(mpi,0) sum += temp;
	}
	Mif(mpi,0){
		double parallel = mpi.time() - time;
		printf("[PARALLEL %d]\tPI = %.20lf at rank %d, taking %lf time (speedup = %lf)\n",mpi.size(),sum,mpi.rank(),parallel,serial/parallel);
	}


}
