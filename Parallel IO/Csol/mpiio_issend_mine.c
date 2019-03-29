#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

#include "ioutils.h"

/*
 *  The global data size is NX x NY
 */

#define NX 480
#define NY 216

/*
 *  The processes are in a 2D array of dimension XPROCS x YPROCS, with
 *  a total of NPROCS processes
 */

#define NDIM 2

#define XPROCS 2
#define YPROCS 2

#define NPROCS (XPROCS*YPROCS)

/*
 *  The local data size is NXP x NYP
 */

#define NXP (NX/XPROCS)
#define NYP (NY/YPROCS)

/*
 *  The maximum length of a file name
 */

#define MAXFILENAME 64

void func(unsigned verbose)
{
  /*
   *  pcoords stores the grid positions of each process
   */

  int pcoords[NPROCS][NDIM];
  
  /*
   *  buf is the large buffer for the master to read into
   *  x contains the local data only
   */

  double buf[NX][NY];
  double x[NXP][NYP];

  int rank, size;
  int i, j;

  int istart, jstart;

  char filename[MAXFILENAME];

  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  /*
   *  Check we are running on the correct number of processes
   */

  if (size != NPROCS)
    {
      if (rank == 0)
	{
	  printf("ERROR: compiled for %d process(es), running on %d\n",
		 NPROCS, size);
	}

      MPI_Finalize();
      exit(-1);
    }

  /*
   *  Work out the coordinates of all the processes in the grid and
   *  print them out
   */

  initpgrid(pcoords, XPROCS, YPROCS);

  if (rank == 0 && verbose)
    {
      printf("Running on %d process(es) in a %d x %d grid\n",
	     NPROCS, XPROCS, YPROCS);
      printf("\n");

      for (i=0; i < NPROCS; i++)
	{
	  printf("Process %2d has grid coordinates (%2d, %2d)\n",
		 i, pcoords[i][0], pcoords[i][1]);
	}
      printf("\n");
    }

  /*
   *  Initialise the arrays to a grey value
   */

  initarray(buf, NX,  NY );
  initarray(x  , NXP, NYP);


  /*
   *  Read the entire array on the master process
   *  Passing "-1" as the rank argument means that the file name has no
   *  trailing "_rank" appended to it, ie we read the global file
   */

  if (rank == 0)
    {
      createfilename(filename, "cinput", NX, NY, -1);
      ioread (filename, buf, NX*NY);
      if(verbose) printf("\n");
    }

  MPI_Datatype my_type_vector;
  MPI_Type_vector(NXP, NYP, NY, MPI_DOUBLE, &my_type_vector);
  MPI_Type_commit(&my_type_vector);

  MPI_Status status;

  if(rank==0){
  
    MPI_Request request;
    for(int dest=0; dest<size; dest++){
      int istart = pcoords[dest][0]*NXP;
      int jstart = pcoords[dest][1]*NYP;
      MPI_Issend(&(buf[istart][jstart]), 1, my_type_vector, dest, dest, comm, &request);
    }
    MPI_Recv(x, NXP*NYP, MPI_DOUBLE, 0, rank, comm, &status);
    MPI_Wait(&request, &status);
  }
  else
    MPI_Recv(x, NXP*NYP, MPI_DOUBLE, 0, rank, comm, &status);

  /*
   *  Every process writes out its local data array x to an individually
   *  named file which has the rank appended to the file name
   */

  createfilename(filename, "coutput", NXP, NYP, rank);
  iowrite(filename, x, NXP*NYP);
}

int main(int argc, char** argv)
{

  unsigned N = (argc>1 ? atoi(argv[1]) : 1);
  unsigned verbose = (argc>2 ? atoi(argv[2]) : 1);

  MPI_Init(NULL, NULL);

  printf("Doing %u iterations\n",N);
  for(unsigned i=0; i<N; ++i)
  	func(verbose);

  MPI_Finalize();

  return 0;
}