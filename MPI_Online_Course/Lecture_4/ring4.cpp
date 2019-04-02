
#include "mpi.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <cmath>
#include <iomanip> //setprecision

/*
Allreduce
SIZE | Time for 1M*2 iterations
1		0.1s
2		0.5s
3		0.5s
4		0.6s
5		0.8s
6		1.0s
7		1.0s

Average: ~[0.25us-0.1us]
Latency (from pingpong exercise): ~0.3us

BEST PERFORMANCE SO FAR
*/

/*
Reduce + Bcast
SIZE | Time for 1M*2 iterations
1		0.2s
2		0.8s
3		1.6s
4		2.2s
5		3.3s
6		1.60s
7		1.65s

Average: ~[0.4us-0.4us]
Latency (from pingpong exercise): ~0.3us

Slower for 1 core;
Performance boost for 6 and 7 cores (and >7 probably).
Similar to Send_recv and Send. Close to latency.
*/

int main(){
	MPI_Init(NULL,NULL);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);



	int next = (rank+1)%size;
	int prev = (rank+size-1)%size;

	MPI_Request request;
	MPI_Status status;

	int sum, sum2;
	int send1, send2;

	const unsigned repeat = 1000000;

	MPI_Barrier(MPI_COMM_WORLD);
	double time = MPI_Wtime();

	for(unsigned k=0; k<repeat; ++k){
		sum = rank;
		sum2 = (rank+1)*(rank+1);
		
		send1 = sum;
		send2 = sum2;

		int recv;

		// MPI_Reduce(&send1, &recv, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		// MPI_Bcast (&recv, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Allreduce(&send1, &recv, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
		sum = recv;

		// MPI_Reduce(&send2, &recv, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		// MPI_Bcast (&recv, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Allreduce(&send2, &recv, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
		sum2 = recv;
	}
	
	printf("[%d/%d] sum = %d;\tsum2 = %d\n",rank,size,sum,sum2);
	
	MPI_Barrier(MPI_COMM_WORLD);
	time = MPI_Wtime() - time;

	if(rank==0)
		printf("Time elapsed: %lf\n",time);

	if(rank==0)
		printf("Final results should be: sum = %d;\tsum2 = %d\n",size*(size-1)/2,size*(size+1)*(2*size+1)/6);

	MPI_Finalize();
}