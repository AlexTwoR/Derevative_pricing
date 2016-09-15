#include "SimpleOptionMC.h"

SimpleOptionMC::SimpleOptionMC(const Bridge& thePayoff_, double Time_) 
	: thePayoff(thePayoff_), Time(Time_) { } 

double SimpleOptionMC::optionPayoff(double Spot) const
{
	return thePayoff(Spot);
} 

double SimpleOptionMC::getTime() const
{
	return Time;
}