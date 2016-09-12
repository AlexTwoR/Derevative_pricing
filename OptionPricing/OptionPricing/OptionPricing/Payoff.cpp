#include "Payoff.h" 

PayoffCall::PayoffCall(double Strike_) : Strike(Strike_){} 

double PayoffCall::operator()(double Spot) const { 

	double result = (Spot > Strike) ? (Spot - Strike) : 0; 
	
	return result; 
} 

Payoff* PayoffCall::clone() const{ 
	//Copy constructor is called, ok since no dynamic objects 
	return new PayoffCall(*this); 
}