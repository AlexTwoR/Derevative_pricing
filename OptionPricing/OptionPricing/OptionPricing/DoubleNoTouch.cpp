#include "DoubleNoTouch.h" 

PayOffDoubleNoTouch::PayOffDoubleNoTouch(double LowerBarrier_, double UpperBarrier_) 
	: LowerBarrier(LowerBarrier_), UpperBarrier(UpperBarrier_)
{} 

double PayOffDoubleNoTouch::operator()(double Spot) const { 

	if (Spot <= LowerBarrier) 
		return 0.0; 

	if (Spot >= UpperBarrier) 
		return 0.0; 
	
	return 100.0; 
} 

Payoff* PayOffDoubleNoTouch::clone() const{ 
	//Copy constructor is called, ok since no dynamic objects 
	return new PayOffDoubleNoTouch(*this); 
}