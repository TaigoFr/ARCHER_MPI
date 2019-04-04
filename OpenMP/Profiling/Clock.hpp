#ifndef _CLOCK_
#define _CLOCK_

#include <chrono>
#include <string>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> chrono_time;

class Clock {
public:
	enum state {start,stop,pause};
	
	Clock(const std::string pstr="",const state ps=start);
	double Start();
	double Lap(const std::string str2="");
	double Stop(const std::string str2="", const std::string str3="");
	double Pause(const std::string str2="", const std::string str3="");
	double Restart(const std::string str2="", const std::string str3="");

	double S(const std::string str2="", const std::string str3="");
	double L(const std::string str2="")								{return Lap	   (str2);	   }
	double P(const std::string str2="", const std::string str3="")	{return Pause  (str2,str3);}
	double R(const std::string str2="", const std::string str3="")	{return Restart(str2,str3);}

	double operator()(const std::string str2="", const std::string str3=""){return S(str2,str3);}

	double GetT(){return T;}
	double Gett(){return t;}
	state GetState(){return s;}

	void setVerbose(bool);

	double count();

	//bias (due to calling Clock): ~50ns (0.05us)
	template<typename T_out, typename ... ARGS1, typename ... ARGS2>
	static void stats(unsigned N, T_out (*func)(ARGS1 ...), ARGS2 && ... args);

	static void sleep(double secs);
private:
	chrono_time chrono_clock();
	chrono_time init;

	double T,t;
	std::string str;
	state s;
	bool verbose;
};

#include "Clock.impl.hpp"

#endif