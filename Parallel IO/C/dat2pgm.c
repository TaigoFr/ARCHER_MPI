/*
 * For use with cioview, compile using: cc -D ORDER_C -o cdat2pgm dat2pgm.c
 * For use with fioview, compile using: cc -D ORDER_F -o fdat2pgm dat2pgm.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXCHAR 32

#define INTMIN   0
#define INTMAX 255

int main(int argc, char **argv)
{
  char *command, *filename, *tmpname;

  double *inbuf;
  double max, min, val;
  char  *outbuf;

  char c;

  FILE *fp;

  int M, N, insize, outsize, i, j, k, ibit, nbit, ival;

  command = argv[0];

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <filename>\n", command);
    exit(1);
  }

  filename = argv[1];

  tmpname = filename;

  while (*tmpname < '0' || *tmpname > '9')
  {
    tmpname++;
  }

  if (sscanf(tmpname, "%04dx%04d.dat", &M, &N) != 2)
  {
    fprintf(stderr, "%s: error parsing filename <%s>\n", command, filename);
    exit(1);
  }

  insize  = M*N*sizeof(double);
  outsize = N*M*sizeof(char);

  if ( (inbuf=(double *) malloc(insize)) == NULL)
  {
    fprintf(stderr, "%s: failed to malloc space for %d x %d input data\n", command, M, N);
    exit(1);
  }

  if ( (outbuf=(char *) malloc(outsize)) == NULL)
  {
    fprintf(stderr, "%s: failed to malloc space for %d x %d output data\n", command, M, N);
    exit(1);
  }

  if ( (fp = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr, "%s: failed to open input file <%s>\n", command, filename);
    exit(1);
  }

  if (fread(inbuf, sizeof(double), M*N, fp) != M*N) 
  {
    fprintf(stderr, "%s: error reading input file <%s>\n", command, filename);
    exit(1);
  }

  fclose(fp);

  min = inbuf[0];  
  max = inbuf[0];  

  for (i=1; i < M*N; i++)
  {
    if (fabs(inbuf[i]) > max) max = fabs(inbuf[i]);
    if (fabs(inbuf[i]) < min) min = fabs(inbuf[i]);
  }

  if (min == max)
    {
      min = 0.0;
      max = 1.0;
    }

  k = 0;

  for (j=N-1; j >=0; j--)
  {
    for (i=0; i < M; i++)
    {

#if defined ORDER_C
      val = fabs(inbuf[i*N+j]); /* C style */
#elif defined ORDER_F
      val = fabs(inbuf[j*M+i]); /* F style */
#else
#error "Must define either ORDER_C or ORDER_F"
#endif

      outbuf[k] = (int) (((double) INTMIN) +
			 (val-min)*(((double)(INTMAX-INTMIN))/(max-min)) + 0.5);
      k++;

    }
  }

  fp = stdout;

  fprintf(fp, "P5\n%d\n%d\n%d\n", M, N, INTMAX);

  if (fwrite(outbuf, sizeof(char), outsize, fp) != outsize)
  {
    fprintf(stderr, "%s: error writing output file <%s>\n", command, filename);
    exit(1);
  }
}
