#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h> 

#define NPOINTS 2000
#define MAXITER 2000


struct complex{
	double real;
	double imag;
};

int main(){
	int numoutside = 0;
	struct complex z, c;

/*
 *   
 *
 *     Outer loops run over npoints, initialise z=c
 *
 *     Inner loop has the iteration z=z*z+c, and threshold test
 */

#pragma omp parallel reduction(+:numoutside), private(z,c), default(none)
{
	int imin = omp_get_thread_num()*NPOINTS/omp_get_num_threads();
	int imax = (omp_get_thread_num()+1)*NPOINTS/omp_get_num_threads();
	printf("From %d to %d in process %d/%d\n",imin,imax,omp_get_thread_num(),omp_get_num_threads());
	for (int i=imin; i<imax; i++){
		for (int j=0; j<NPOINTS; j++){
			c.real = -2.0+2.5*(double)(i)/(double)(NPOINTS)+1.0e-7;
			c.imag = 1.125*(double)(j)/(double)(NPOINTS)+1.0e-7;
			z = c;
			for (int iter=0; iter<MAXITER; iter++){
				double ztemp  = (z.real*z.real) - (z.imag*z.imag) + c.real;
				z.imag = z.real*z.imag*2 + c.imag; 
				z.real = ztemp; 
				if ((z.real*z.real + z.imag*z.imag)>4.0e0) {
					++numoutside;
					break;
				}
			}
		}
	}
}

/*
 *  Calculate area and error and output the results
 */

	double area, error;
	area  = 2.0*2.5*1.125*(double)(NPOINTS*NPOINTS-numoutside)/(double)(NPOINTS*NPOINTS);
	error = area/(double)NPOINTS;

	printf("Area of Mandlebrot set = %12.8f +/- %12.8f (%d points)\n",area,error,numoutside);

}
