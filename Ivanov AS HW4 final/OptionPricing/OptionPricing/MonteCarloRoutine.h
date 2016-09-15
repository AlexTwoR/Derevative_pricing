#ifndef MONTECARLOROUTINE_H 
#define MONTECARLOROUTINE_H 
#include "SimpleOptionMC.h"
#include "OptionProperties.h"

class MonteCarloRoutine {
	
public:

	double Spot;
	double Strike;
	double Rate; //in % annualized 
	double Vol; //in % annualized 
	double Time; //time to maturity in years
	double Price;

	int numberOfPaths;
	OptionType Type;

	MonteCarloRoutine(double S, double X, double r, double vol, double T, int numberOfPaths, OptionType type);

	void MC();
	double getPrice() const;

private:
	static double OneTouch(const double& val);

};


#endif