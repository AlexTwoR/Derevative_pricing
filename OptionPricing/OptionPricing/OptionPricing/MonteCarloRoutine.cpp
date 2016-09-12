#include "MonteCarloRoutine.h"
#include "DoubleNoTouch.h"
#include "Random.h"
#include <cmath>


MonteCarloRoutine::MonteCarloRoutine(double Spot_, double Strike_, double Rate_, double Vol_, double Time_, int _numberOfPaths, OptionType _type) :
	Spot(Spot_), Strike(Strike_), Time(Time_), Type(_type), numberOfPaths(_numberOfPaths) {

	Rate = Rate_ / 100.0;
	Vol = Vol_ / 100.0;

	MC();

}


void MonteCarloRoutine::MC()
{
	int signTypeOfExp = Type == Call ? 1 : -1;
	double S_adjust = Spot * exp(Time*(Rate - 0.5*Vol*Vol));
	double S_cur = 0.0;
	double payoff_sum = 0.0;

	for (int i = 0; i<numberOfPaths; i++) {
		double gauss_bm = RandGaussian();
		S_cur = S_adjust * exp(sqrt(Vol*Vol*Time)*gauss_bm);
		payoff_sum += OneTouch((S_cur - Strike)*signTypeOfExp);
	}

	Price = (payoff_sum / static_cast<double>(numberOfPaths)) * exp(-Rate*Time);

}

double MonteCarloRoutine::OneTouch(const double& val)
{
	if (val >= 0) 
		return 100.0;	
	else 
		return 0.0;	
}


double MonteCarloRoutine::getPrice() const
{
	return Price;
}