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

#define XPROCS 3
#define YPROCS 3

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

  char filename[MAXFILENAME];

  /*
   *  Variables needed for MPI-IO
   */

  int sizes[NDIM], subsizes[NDIM], starts[NDIM];

  MPI_Datatype my_mpi_subarray;

  MPI_File fh;
  MPI_Status status;

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
   *  Define the NXP x NYP subarray for this process
   */

  sizes[0] = NX;
  sizes[1] = NY;

  subsizes[0] = NXP;
  subsizes[1] = NYP;

  starts[0] = pcoords[rank][0] * NXP;
  starts[1] = pcoords[rank][1] * NYP;

  MPI_Type_create_subarray(NDIM, sizes, subsizes, starts,
                           MPI_ORDER_C, MPI_DOUBLE, &my_mpi_subarray);

  MPI_Type_commit(&my_mpi_subarray);


  /*
   *  Construct name of input file: -1 means no rank info is appended
   */

  createfilename(filename, "cinput", NX, NY, -1);


  /*
   *  Open the file for reading only and attach to file handle fh
   *  No IO hints are passed since MPI_INFO_NULL is specified
   */

  if (MPI_File_open(comm, filename, MPI_MODE_RDONLY,
		    MPI_INFO_NULL, &fh) != MPI_SUCCESS)
    {
      printf("Open error on rank %d\n", rank);
    }

  /*
   *  Set view for this process using appropriate datatype
   */

  if (MPI_File_set_view(fh, 0, MPI_DOUBLE, my_mpi_subarray, "native",
			MPI_INFO_NULL) != MPI_SUCCESS)
    {
      printf("View error on rank %d\n", rank);
    }

  /*
   *  Read all the data for this process (ie NXP*NYP doubles)
   */

  if (MPI_File_read_all(fh, x, NXP*NYP, MPI_DOUBLE, &status) != MPI_SUCCESS)
    {
      printf("Read error on rank %d\n", rank);
    }

  /*
   *  Close file
   */

  if (MPI_File_close(&fh) != MPI_SUCCESS)
    {
      printf("Close error on rank %d\n", rank);
    }

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