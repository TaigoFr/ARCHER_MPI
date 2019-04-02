
#include "mpi.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <cmath>
#include <iomanip> //setprecision

/*
N=10
SIZE | Time for 1M*2*N iterations
1		0.2s
2		0.6s
3		0.8s
4		0.9s
5		1.25s
6		1.35s
7		1.65s

Average: ~[0.03us-0.01us]
Latency (from pingpong exercise): ~0.3us

Good performance. To be compared with reduce_parts.cpp
It almost seems as if the collective reduction operation is seen as a single operation (without the factor of 10 being needed)
*/

int main(){
	MPI_Init(NULL,NULL);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	const int N = 10;
	int vec[N];
	for(int i=0; i<N; ++i)
		vec[i] = rank;

	int next = (rank+1)%size;
	int prev = (rank+size-1)%size;

	MPI_Request request;
	MPI_Status status;

	int sum[N], sum2[N];
	int send1[N], send2[N];

	const unsigned repeat = 1000000;

	MPI_Barrier(MPI_COMM_WORLD);
	double time = MPI_Wtime();

	for(unsigned k=0; k<repeat; ++k){
		for(int i=0; i<N; ++i){
			sum[i] = rank;
			sum2[i] = (rank+1)*(rank+1);
		
			send1[i] = sum[i];
			send2[i] = sum2[i];
		}

		int recv[N];

		MPI_Allreduce(&send1, &recv, N, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
		for(int i=0; i<N; ++i)
			sum[i] = recv[i];

		MPI_Allreduce(&send2, &recv, N, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
		for(int i=0; i<N; ++i)
			sum2[i] = recv[i];
	}
	
	printf("[%d/%d] sum[5] = %d;\tsum2[5] = %d\n",rank,size,sum[5],sum2[5]);
	
	MPI_Barrier(MPI_COMM_WORLD);
	time = MPI_Wtime() - time;

	if(rank==0)
		printf("Time elapsed: %lf\n",time);

	if(rank==0)
		printf("Final results should be: sum = %d;\tsum2 = %d\n",size*(size-1)/2,size*(size+1)*(2*size+1)/6);

	MPI_Finalize();
}