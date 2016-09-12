#ifndef OPTIONCLASS_H 
#define OPTIONCLASS_H 
#include "Bridge.h" 
//Abstract class for Option - defined interface, overload payoff function in inherited classes //to return payoff at expiry 

class SimpleOptionMC {
public: 
	SimpleOptionMC(const Bridge& thePayoff_, double Time_); 
	double optionPayoff(double Spot) const; 
	double getTime() const; 

private:
	double Time; 
		//Bridge to separate Option class from Payoff class realization 
	Bridge thePayoff; 
};

#endif