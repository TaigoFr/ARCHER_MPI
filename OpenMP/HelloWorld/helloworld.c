#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(){
#pragma omp parallel
    {
#pragma omp critical 
     {
      printf("hello from thread %d\n",omp_get_thread_num());
     }
    }
}
