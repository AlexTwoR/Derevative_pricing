#ifndef DOUBLENOTOUCH_H 
#define DOUBLENOTOUCH_H 
#include "Payoff.h" 

class PayOffDoubleNoTouch : public Payoff
{
	public: 
		PayOffDoubleNoTouch(double LowerBarrier_, double UpperBarrier_); 
		virtual double operator()(double Spot) const; 

		virtual ~PayOffDoubleNoTouch(){} 
		
		Payoff* clone() const;

	private: 
		double LowerBarrier; 
		double UpperBarrier;

};

#endif