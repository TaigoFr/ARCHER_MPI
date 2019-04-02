
#include "mpi.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <cmath>
#include <iomanip> //setprecision

/*
SIZE | Time for 1M iterations
1		0.003s
2		0.5s
3		1.2s
4		2s
5		4s
6		4.5s
7		~5.7s

Average: ~[0.25us-0.5us]
Latency (from pingpong exercise): ~0.3us

A lot fast than using Issend/Recv/Wait. Almost double speed. Close to latency.
*/

int main(){
	MPI_Init(NULL,NULL);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);



	int next = (rank+1)%size;
	int prev = (rank+size-1)%size;

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

		for(int i=1; i<size; ++i){
			int recv;

			MPI_Sendrecv(&send1, 1, MPI_INT, next, rank, &recv, 1, MPI_INT, prev, prev, MPI_COMM_WORLD, &status);

			sum  += recv;
			send1 = recv;

			MPI_Sendrecv(&send2, 1, MPI_INT, next, rank, &recv, 1, MPI_INT, prev, prev, MPI_COMM_WORLD, &status);

			sum2 += recv;
			send2 = recv;

			// printf("[%d/%d] [step %d] sum = %d;\tsum2 = %d\n",rank,size,i,sum,sum2);
		}
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