#include "OptionClass.h" 
#include "Random.h" 
#include <cmath> //Function definition 


VanillaOption::VanillaOption(double Spot_, double Strike_, double Rate_, double DivY_, double Vol_, double Time_, OptionType _type) :
Spot(Spot_), Strike(Strike_),Time(Time_), Type(_type){

    Rate = Rate_ / 100.0; 
    Vol = Vol_ / 100.0; 
	DivYield = DivY_ / 100.0;

} 

double VanillaOption::getPrice() const{

    double price = 0.0; 

	double d1 = (log(Spot / Strike) + (Rate - DivYield + Vol * Vol / 2.0) * Time) / (Vol * sqrt(Time));
	double d2 = d1 - Vol * sqrt(Time);

	if(Type == Call)
		price = Spot * exp(-DivYield * Time) * CDF_HA(d1) - Strike * exp(-Rate * Time) * CDF_HA(d2);
	else
		price = -Spot * exp(-DivYield * Time) * CDF_PA(-d1) + Strike * exp(-Rate * Time) * CDF_PA(-d2);

	return price; 
} 

double VanillaOption::getDelta() const{

    double d1 = (log(Spot / Strike) + (Rate - DivYield + Vol * Vol / 2.0) * Time) / (Vol * sqrt(Time)); 

	if (Type == Call)
		return CDF_PA(d1) *	exp(-DivYield * Time);
	else
		return 1 - CDF_PA(d1) * exp(-DivYield * Time);
}

double VanillaOption::getGamma() const {

	double d1 = (log(Spot / Strike) + (Rate - DivYield + Vol * Vol / 2.0) * Time) / (Vol * sqrt(Time));

	return PDF(d1) * exp(-DivYield * Time) / (Spot * Vol * sqrt(Time));
}

double VanillaOption::getVega() const {

	double d1 = (log(Spot / Strike) + (Rate - DivYield + Vol * Vol / 2.0) * Time) / (Vol * sqrt(Time));

	return  Spot*sqrt(Time)*PDF(d1)*exp(-DivYield*Time);
}

double VanillaOption::getTheta() const {

	double d1 = (log(Spot / Strike) + (Rate - DivYield + Vol * Vol / 2.0) * Time) / (Vol * sqrt(Time));
	double d2 = d1 - Vol * sqrt(Time);


	if (Type == Call)
		return -Spot * PDF(d1) * Vol * exp(-DivYield * Time)/(2 * sqrt(Time))
		- Rate * Strike * exp(-Rate*Time)* CDF_PA(-d2)
		+ DivYield*Spot * CDF_PA(d1) * exp(-DivYield * Time);
	else
		return -Spot * PDF(d1) * Vol * exp(-DivYield * Time)
		/ (2 * sqrt(Time)) - DivYield * Spot * CDF_PA(-d1) * exp(-DivYield * Time)
		+ Rate* Strike * exp(-Rate * Time) * CDF_PA(-d2);

}

double VanillaOption::getRho() const {

	double d1 = (log(Spot / Strike) + (Rate - DivYield + Vol * Vol / 2.0) * Time) / (Vol * sqrt(Time));
	double d2 = d1 - Vol * sqrt(Time);

	if (Type == Call)
		return Strike*Time*exp(-Rate * Time)*CDF_HA(d2);
	else
		return -Strike*Time*exp(-Rate * Time)*CDF_HA(-d2);

}