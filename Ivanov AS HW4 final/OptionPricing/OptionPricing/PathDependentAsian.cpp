

#include "PathDependentAsian.h"

PathDependentAsian::PathDependentAsian(double Time_,const Bridge& ThePayOff_, std::vector<double> averagingTimes_) 
	: Time(Time_), thePayoff(ThePayOff_)
{
	averagingTimes.swap(averagingTimes_);
} 

double PathDependentAsian::optionPayoff(double Spot) const
{
	return thePayoff(Spot);
} 

double PathDependentAsian::getTime() const
{
	return Time;
}

std::vector<double> PathDependentAsian::getAveragingTimes() const
{
	return averagingTimes;
}