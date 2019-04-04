
#include "Clock.hpp"

#define N1 1e9
#define N2 3e7

void f1(){ for(int i=0; i<N1; i++) i=i; }
void f2(){ for(int i=0; i<N2; i++) i=i; }
void f3(){ f1(); }
void f4(){ f2(); }


int main(){

	double time = (float)clock()/CLOCKS_PER_SEC;
	f1();
	time = (float)clock()/CLOCKS_PER_SEC - time;
	printf("Time 1: %lfs\n\n",time);

	time = (float)clock()/CLOCKS_PER_SEC;
	f2();
	time = (float)clock()/CLOCKS_PER_SEC - time;
	printf("Time 2: %lfs\n\n",time);

	time = (float)clock()/CLOCKS_PER_SEC;
	f3();
	time = (float)clock()/CLOCKS_PER_SEC - time;
	printf("Time 3: %lfs\n\n",time);

	time = (float)clock()/CLOCKS_PER_SEC;
	f4();
	time = (float)clock()/CLOCKS_PER_SEC - time;
	printf("Time 4: %lfs\n\n",time);

	return 0;
}