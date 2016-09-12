#pragma once
#ifndef AMERICANOPTION_H 
#define AMERICANOPTION_H 

#include "OptionClass.h" 
#include "OptionProperties.h"
#include <vector>

class AmericanOption : public Option 
{ 
public: 
	AmericanOption(double S, double X, double r, double d, double vol, double T, OptionType type);
	virtual double getPrice() const; 
	virtual double getDelta() const; 
	virtual double getGamma() const; 
	virtual double getVega() const; 
	virtual double getTheta() const; 
	virtual double getRho() const;

	void CalculateGrid();

	double Spot; 
	double Strike;
	double Rate; //in % annualized 
	double Vol; //in % annualized 
	double Time; //time to maturity in years
	int TimeSteps; //number of steps 
	double deltaT; //step length 
	double DivYield;
	OptionType Type;

private:
	int UnderlyingSteps; //number of steps 
	double deltaX; //step length 
	std::vector < std::vector<double> > Grid; //pricing grid 
	std::vector <double> SpotArray; //possible spot values 
	int SpotPosition; //position in SpotArray corresponding to the first element greater than Spot 
}; 

#endif