#ifndef BARRIEROPTIONCN_H 
#define BARRIEROPTIONCN_H 
#include "OptionClass.h" 
#include <vector> 

class AmericanBarrier : public Option 
{ 
public: 
	AmericanBarrier(double S, double X, double R, double V, double t, double Div, double Barr, OptionType type);
	virtual double getPrice() const; 
	virtual double getDelta() const; 
	virtual double getGamma() const;
	virtual double getVega() const; 
	virtual double getTheta() const; 
	virtual double getRho() const;

//private: 
	double Spot; 
	double Strike; 
	double Rate; //in % annualized 
	double Vol; //in % annualized 
	double Time; //time to maturity in years 
	double DividendYield; //in % annualized
	double Barrier; 
	double Price;
	OptionType Type;

	void CalculateGrid();

private:
	std::vector <std::vector <double> > inverse(std::vector <std::vector <double> > a) const;
	int TimeSteps; //number of steps 
	double dt; //step length 
	int UnderlyingSteps; //number of steps 
	double dS; //step length 
	std::vector < std::vector<double > > Grid; //pricing grid 
	std::vector <double> SpotArray; //possible spot values 
	int SpotPosition; //position in SpotArray corresponding to the first element greater than Spot 
}; 

#endif