
#if !defined(_CLOCK_)
#error "This file should only be included through Clock.hpp"
#endif

#include <cstdio>
#include <cmath> //sqrt
#include <unistd.h> //usleep

Clock::Clock(const std::string pstr,const state ps):init(chrono_clock()),T(0.),t(0.),str(pstr),s(ps), verbose(true){
	if(ps==start) Start();
}


double Clock::Start(){
	double temp=count();
	if(s==pause)
		T=temp-(t-T);
	else
		T=temp;
	t=T;
	s=start;
	return 0;
}
double Clock::Lap(const std::string str2){
	if(s!=start) return t;
	double temp=count();
	if(verbose) printf("%s%s\t%lf %lf\n",str.c_str(),str2.c_str(),temp-T,temp-t);
	t=temp;
	return t-T;
}

double Clock::Stop(const std::string str2, const std::string str3){
	if(s==stop) return t;
	double temp= (s==pause) ? t : count();
	if(verbose) printf("%s%s%lf%s",str.c_str(),str2.c_str(),temp-T,str3.c_str());
	s=stop;
	return temp-T;
}

double Clock::Pause(const std::string str2, const std::string str3){
	if(s!=start) return t;
	double temp=count();
	if(verbose) printf("%s%s%lf%s",str.c_str(),str2.c_str(),temp-T,str3.c_str());
	t=temp;
	s=pause;
	return t-T;
}
double Clock::Restart(const std::string str2, const std::string str3){
	if(s!=stop) S(str2,str3);
	return S(str2,str3);
}

//assumes Start if s=pause
double Clock::S(const std::string str2, const std::string str3){
	if(s==stop) 	return Start();
	else 			return Stop(str2,str3);
}

void Clock::setVerbose(bool v) { verbose = v; }

chrono_time Clock::chrono_clock(){
	return std::chrono::high_resolution_clock::now();
}

double Clock::count(){
	auto temp = chrono_clock();
	std::chrono::duration<double> elapsed = temp - init;
	return elapsed.count();
}


template<typename T_out, typename ... ARGS1, typename ... ARGS2>
void Clock::stats(unsigned N, T_out (*func)(ARGS1 ...), ARGS2 && ... args){
	if(N==0) return;
	double sum = 0., sum2 = 0.;
	// double sum3 = 0., sum4 = 0.;

	Clock C("",stop);
	C.setVerbose(false);
	for(unsigned i=0; i<N; i++){
		C();
		func(args...);
		double t = C();
		sum += t;
		sum2 += t*t;
		// sum3 += t*t*t;
		// sum4 += t*t*t*t;
	}
	sum /= N; //mean
	if(N>1){
		sum2 = sqrt(sum2/(N-1.) - sum*sum*N/(N-1.)); //population/sample standard deviation

		// printf("%lf\n",sum4);
		// sum2 = sum2/N - sum*sum; //variance (not corrected)
		// sum4 = sum4/N - 4.*sum*sum3/N + 6.*sum*sum*sum2 - 3.*sum*sum*sum*sum; //4th moment experimental!!!!!!!!
		// printf("%.10lf %.10lf\n",sum4,sum2*sum2);
		// sum4 = sum2*(1./(4.*N)*(sum4/(sum2*sum2)-1.));// + 1./(2.*N*N) + 3./(64.*N*N)*(sum4/(sum2*sum2)-1.)*(sum4/(sum2*sum2)-1.)); //variance of sample standard deviation up to O(1/N^3)
		// printf("%lf\n",sum4);


		// sum2 = sqrt(sum2*N/(N-1.)); //population standard deviation
		// sum4 = sqrt(sum4); //standard deviation of sample standard deviation
	}

	printf("TIME\t = %lf (%u iterations)\n",sum*N,N);
	printf("AVERAGE\t = %lf\n",sum);
	// if(N>1)	printf("STD.DEV. = %lf +- %lf (%lf%% | %lf)\n",sum2,sum4,sum2/sum,sum4/sum2);
	if(N>1)	printf("STD.DEV. = %lf (%lf%%)\n",sum2,sum2/sum);
}

void Clock::sleep(double secs){
	usleep(secs*1.e6);
}