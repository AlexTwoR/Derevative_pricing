#pragma once
#pragma once
#ifndef BERMUDANOPTION_H 
#define BERMUDANOPTION_H  

#include "OptionClass.h" 
#include "OptionProperties.h"
#include <vector>

class BermudanOption : public Option
{
public:
	BermudanOption(double S, double X, double r, double d, double vol, double T, std::vector < std::vector<double> > Ss, OptionType type);
	virtual double getPrice() const;
	virtual double getDelta() const;
	virtual double getGamma() const;
	virtual double getVega() const;
	virtual double getTheta() const;
	virtual double getRho() const;

	void CalculateGrid();
	int isStrikeDay(double); //sent time in double from initial

	double Spot;
	double Strike;
	double Rate; //in % annualized 
	double Vol; //in % annualized 
	double Time; //time to maturity in years
	std::vector < std::vector<double> > TimeStrike;
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