#ifndef OPTIONCLASS_H 
#define OPTIONCLASS_H //Abstract class for Option - defined interface, overload payoff function in inherited classes 

#include "OptionProperties.h"

class Option { 
    public: 
        Option(){}; 
        virtual double getPrice() const = 0; 
        virtual double getDelta() const = 0; 
        virtual double getGamma() const = 0; 
        virtual double getVega() const = 0; 
        virtual double getTheta() const = 0;
		virtual double getRho() const = 0;

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
}; 




class VanillaOption : public Option 
{ 
public:
	VanillaOption(double S, double X, double r, double d, double vol, double T, OptionType type);
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
	double DivYield; //in % annualized 
    double Vol; //in % annualized 
    double Time; //time to maturity in years
	OptionType Type;
}; 
#endif
