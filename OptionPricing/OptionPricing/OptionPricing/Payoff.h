#ifndef PAYOFF_H 
#define PAYOFF_H 

//Abstract class to define the interface 
class Payoff {
public: 
	Payoff(){}; 
	virtual ~Payoff(){} 
	virtual double operator()(double Spot) const = 0; 
	virtual Payoff* clone() const = 0; 
}; 

class PayoffCall : public Payoff
{
	public: PayoffCall(double Strike_); 
			double operator()(double Spot) const; 
			~PayoffCall(){} Payoff* clone() const;

	private: 
		double Strike;
}; 

#endif