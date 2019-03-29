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

void func()
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

  int rank, size, dest, tag;
  int i, j;

  MPI_Status status;

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

  if (rank == 0)
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
      printf("\n");
    }

  tag = 0;

  if (rank == 0)
    {

      /*
       *  Send data to each of the workers in turn using synchronous sends
       *  Do them in reverse order of rank so that the code is slightly
       *  neater - the master does the copy but omits the send to itself.
       *  Tag all messages with tag=0
       */

      for (dest = size-1; dest >= 0; dest--)
	{
	  

  /*
   *  Copy down the correct data from buf to x. Need to work out, using the
   *  position in the process grid, what the index of the bottom-left-hand
   *  pixel for this particular destiniation process.
   */

	  istart = pcoords[dest][0]*NXP;
	  jstart = pcoords[dest][1]*NYP;

	  for (i=0; i < NXP; i++)
	    {
	      for (j=0; j < NYP; j++)
		{
		  x[i][j] = buf[istart+i][jstart+j];
		}
	    }

	  if (dest != 0)
	    {
	      printf("rank %d sending to rank %d\n", rank, dest);
	      MPI_Ssend(x, NXP*NYP, MPI_DOUBLE, dest, tag, comm);
	    }
	}
    }
  else
    {
      /*
       *  Workers receive data from master
       */

      printf("rank %d receiving from rank 0\n", rank);
      MPI_Recv(x, NXP*NYP, MPI_DOUBLE, 0, tag, comm, &status);
    }

  /*
   *  Every process writes out its local data array x to an individually
   *  named file which has the rank appended to the file name
   */

  createfilename(filename, "coutput", NXP, NYP, rank);
  iowrite(filename, x, NXP*NYP);

}


int main()
{


  MPI_Init(NULL, NULL);

  unsigned N = 1;
  for(unsigned i=0; i<N; ++i)
  	func();

  MPI_Finalize();

  return 0;
}