#include <iostream>
#include "OptionClass.h"
#include "AmericanOption.h"
#include "BermudanOption.h"
#include  "BarrierOption.h"
#include "MonteCarloRoutine.h"
#include "RandomGenerator.h"
#include "Payoff.h"
#include <boost/math/distributions/inverse_gaussian.hpp> 
#include <boost/random.hpp> 


using namespace std;

int main() {

	double S = 100.0;
	double X = 110.0;
	double r = 5.0;
	double divY = 2;
	double vol = 20;
	int days = 365; //time in days
	double t = days / 365.0; //time in double
	OptionType type = Call; // "Call" or "Put" define in OptionProperties
	int numberOfInteration = 2000000;
	//double barrier = 80;
	OptionStyle style = Digital;
	

	VanillaOption option = VanillaOption(S, X, r, divY, vol, t, type);
	//AmericanBarrier option = AmericanBarrier(S, X, r, vol, t, divY, barrier, type);
	//AmericanOption option = AmericanOption(S, X, r, divY, vol, t, type);
	//BermudanOption option = BermudanOption(S, X, r, divY, vol, t, BermudanStrike, type); //example see after code down
	//MonteCarloRoutine option = MonteCarloRoutine(S, X, r, vol, t, numberOfInteration, type);
	

	const int numberOfPaths = 10000000;

	//Create option object, define payoff 
	PayoffCall thePayoff(X);

	//State averaging times 
	//Daily - 252 business days 

	int length = round(t * 252);
	std::vector<double> averagingTimes; 
	averagingTimes.resize(length);

	for (int i = 0; i < length; i++)
	{
		averagingTimes[i] = i * (t / (length - 1));
	}

	std::vector<double> arrayToFillUni;
	arrayToFillUni.resize(5);


	//PathDependentAsian theOption(t, thePayoff, averagingTimes);

	//Decorated generator
	//AntiThetic theDecoratedGenerator(&theGenerator); 

	//double* result = ExoticMC(theOption, theGenerator, S, vol, r, numberOfPaths);

		
	
	//-----Output Data-----
	cout << "BS Option Prising" << endl << endl;

	cout << "Parameters:" << endl;
	switch (style)
	{
	case American:
		cout << "American Option" << endl;
		break;
	case Bermudan:
		cout << "Bermudan Option" << endl;
		break;
	case Barrier:
		cout << "Bermudan Option" << endl;
		break;
	case Digital:
		cout << "Bermudan Option" << endl;
		break;

	default: //European
		cout << "European Option" << endl; 
	}

	if (option.Type == Call)
		cout << "Call" << endl;
	else
		cout << "Put" << endl;

	cout << "S = " << option.Spot << endl;
	cout << "X = " << option.Strike << endl;
	cout << "Days = " << option.Time*365 << endl;
	cout << "Vol = " << option.Vol << endl;
	cout << "r = " << option.Rate << endl;
 	//cout << "Div = " << option.DividendYield << endl;
	cout << endl;

	cout << "Results:" << endl;
	cout << "Price = " << option.getPrice() << endl;
	//cout << "Greeks:" << endl;
	//cout << "Delta = " << option.getDelta() << endl;
	//cout << "Gamma = " << option.getGamma() << endl;
	//cout << "Vega = " << option.getVega()/100 << endl;
	//cout << "Theta = " << option.getTheta()/365 << endl;
	//cout << "Rho = " << option.getRho()/100 << endl;
	cout << endl;

	//if (style == Bermudan) {
	//	cout << "Bermudan striks:" << endl;
	//	if(BermudanStrike[0].size()>0)
	//		for (int i = 0; i < BermudanStrike[0].size(); i++)
	//			cout << "Day: " << BermudanStrike[0][i] * 365
	//			<< " Strike: " << BermudanStrike[1][i] << endl;
	//	else
	//		cout << "No striks" << endl;
	//}

	system("pause");
}


////For Bermudan Option
//std::vector < std::vector<double> > BermudanStrike(2);
////Times
//BermudanStrike[0].push_back(1 / 365.0);
//BermudanStrike[0].push_back(25 / 365.0);
//BermudanStrike[0].push_back(50 / 365.0);
//BermudanStrike[0].push_back(100 / 365.0);
//BermudanStrike[0].push_back(150 / 365.0);

////Striks
//BermudanStrike[1].push_back(101.65);
//BermudanStrike[1].push_back(102.65);
//BermudanStrike[1].push_back(103.65);
//BermudanStrike[1].push_back(104.65);
//BermudanStrike[1].push_back(105.65);