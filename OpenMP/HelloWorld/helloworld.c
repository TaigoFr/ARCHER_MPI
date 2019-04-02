#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(){
    printf("OUT hello from thread %d of %d\n",omp_get_thread_num(),omp_get_num_threads());

#pragma omp parallel
    {
#pragma omp critical 
     {
      printf("IN hello from thread %d of %d\n",omp_get_thread_num(),omp_get_num_threads());
     }
    }
}
