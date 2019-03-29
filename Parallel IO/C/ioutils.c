#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

void createfilename(char *filename, char *basename, int nx, int ny, int rank)
{
  if (rank < 0)
    {
      sprintf(filename, "%s%04dx%04d.dat", basename, nx, ny);
    }
  else
    {
      sprintf(filename, "%s%04dx%04d_%02d.dat", basename, nx, ny, rank);
    }
}


void iosize(char *filename, int *nx, int *ny)
{ 
  char *tmpname;

  tmpname = filename;

  while (*tmpname < '0' || *tmpname > '9')
  {
    tmpname++;
  }

  if (sscanf(tmpname, "%04dx%04d.dat", nx, ny) != 2)
  {
    printf("iosize: error parsing filename <%s>\n", filename);
    exit(1);
  }
}


void ioread(char *filename, void *ptr, int ndouble)
{
  int i;

  FILE *fp;
  
  double *data = (double *) ptr;

  printf("ioread: reading <%s> ...\n", filename);

  if ( (fp = fopen(filename, "r")) == NULL)
  {
    printf("ioread: failed to open input file <%s>\n", filename);
    exit(1);
  }

  if (fread(data, sizeof(double), ndouble, fp) != ndouble) 
  {
    printf("ioread: error reading input file <%s>\n", filename);
    exit(1);
  }

  fclose(fp);

  printf("... done\n");
}

void iowrite(char *filename, void *ptr, int ndouble)
{
  int i;

  FILE *fp;
  
  double *data = (double *) ptr;

  printf("iowrite: writing <%s> ...\n", filename);

  if ( (fp = fopen(filename, "w")) == NULL)
  {
    printf("iowrite: failed to open output file <%s>\n", filename);
    exit(1);
  }

  if (fwrite(data, sizeof(double), ndouble, fp) != ndouble) 
  {
    printf("iowrite: error writing output file <%s>\n", filename);
    exit(1);
  }

  fclose(fp);
  printf("... done\n");
}

#define INITDATAVAL 0.5

void initarray(void *ptr, int nx, int ny)
{
  int i, j;

  double *data = (double *) ptr;

  for (i=0; i < nx*ny; i++)
    {
      data[i] = INITDATAVAL;
    }
}

#define NDIM 2

void initpgrid(void *ptr, int nxproc, int nyproc)
{
  MPI_Comm gridcomm;
  MPI_Comm comm = MPI_COMM_WORLD;

  int dims[NDIM];
  int periods[NDIM] = {0, 0};

  int reorder = 0;

  int *pcoords = (int *) ptr;

  int i;

  dims[0] = nxproc;
  dims[1] = nyproc;

  MPI_Cart_create(comm, NDIM, dims, periods, reorder, &gridcomm);

  for (i=0; i < nxproc*nyproc; i++)
    {
      MPI_Cart_coords(gridcomm, i, NDIM, pcoords+NDIM*i);
    }

  MPI_Comm_free(&gridcomm);

}
