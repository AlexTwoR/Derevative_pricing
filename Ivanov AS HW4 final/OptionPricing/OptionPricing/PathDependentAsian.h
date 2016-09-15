#ifndef PATHDEPENDENTASIAN_H 
#define PATHDEPENDENTASIAN_H 
#include "Bridge.h" 
#include <vector> 

class PathDependentAsian{ 
public: 
	PathDependentAsian(double Time_, const Bridge& ThePayOff_, std::vector<double> averagingTimes_); 
	double optionPayoff(double Spot) const; 
	double getTime() const; 
	std::vector<double> getAveragingTimes() const; 
private: 
	double Time; 
	Bridge thePayoff; 
	std::vector<double> averagingTimes; 	//at these dates from today underlying values will be fixed 

};

#endif