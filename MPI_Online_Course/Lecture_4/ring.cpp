
#include "mpi.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <cmath>
#include <iomanip> //setprecision

/*
SIZE | Time for 1M*2 iterations
1		0.003s
2		1s
3		1.85s
4		3s
5		~5.4s
6		~6.9s
7		~8.1s

Average: ~[0.5us-0.7us]
Latency (from pingpong exercise): ~0.3us

Slower than latency. But close. Probably Issends and Recvs and Waits have to wait for one another
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

		for(int i=1; i<size; ++i){
			int recv;
			MPI_Issend(&send1, 1, MPI_INT, next, rank, MPI_COMM_WORLD, &request);
			MPI_Recv  (&recv , 1, MPI_INT, prev, prev, MPI_COMM_WORLD, &status);
			MPI_Wait  (&request, &status);

			sum  += recv;
			send1 = recv;

			MPI_Issend(&send2, 1, MPI_INT, next, rank, MPI_COMM_WORLD, &request);
			MPI_Recv  (&recv , 1, MPI_INT, prev, prev, MPI_COMM_WORLD, &status);
			MPI_Wait  (&request, &status);

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