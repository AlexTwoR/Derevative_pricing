#ifndef BRIDGE_H #define BRIDGE_H 
#include "Payoff.h" 

//New class created for memory handling and copying 
class Bridge { 
public: 
	//For type conversion Payoff <-> Bridge 
	Bridge(const Payoff& initialPayoff); 
	
	//Copy constructor for 	Bridge 
	Bridge(const Bridge& initial); 
	double operator()(double Spot) const; 

	//Class stores a pointer, memory was allocated with new 
	~Bridge(); 
private: 
	Payoff* thePayoff; 
}; 

#endif