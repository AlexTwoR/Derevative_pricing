#include "ExoticMonteCarlo.h" 
#include "OptionProperties.h"


ExoticMonteCarlo::ExoticMonteCarlo(double Spot_, double Strike_, double Rate_, double Vol_, double Time_, int _numberOfPaths, OptionType _type, 
	AverageType AvType) :	Spot(Spot_), Strike(Strike_), Time(Time_), Type(_type), numberOfPaths(_numberOfPaths) {

	Rate = Rate_ / 100.0;
	Vol = Vol_ / 100.0;

	Bridge thePayoffF;

	if (Type == Call) {
		PayoffCall thePayoff(Strike);
		thePayoffF = thePayoff;
	}
	else {
		PayoffPut thePayoff(Strike);
		thePayoffF = thePayoff;
	}

	int length = round(Time * getAveradeInt(AvType));
	std::vector<double> averagingTimes; averagingTimes.resize(length);

	for (int i = 0; i < length; i++)
	{
		averagingTimes[i] = i * (Time / (length - 1));
	}

	PathDependentAsian theOption(Time, thePayoffF, averagingTimes);
	BasicRandomGenerator theGenerator(121);

	Price = *ExoticMC(theOption, theGenerator);

}


ExoticMonteCarlo::ExoticMonteCarlo(double Spot_, double Spot2_, double Strike_, double Rate_, double Vol_, double Vol2_,
	double Rho_, double Time_, int _numberOfPaths, OptionType _type, AverageType AvType) :
	Spot(Spot_), Spot2(Spot2_), Strike(Strike_), Time(Time_), Type(_type), numberOfPaths(_numberOfPaths)  {

	Rho = Rho_ / 100.0;
	Rate = Rate_ / 100.0;
	Vol = Vol_ / 100.0;
	Vol2 = Vol2_ / 100.0;

	Bridge thePayoffF;

	if (Type == Call) {
		PayoffCall thePayoff(Strike);
		thePayoffF = thePayoff;
	}
	else {
		PayoffPut thePayoff(Strike);
		thePayoffF = thePayoff;
	}

	int length = round(Time * getAveradeInt(AvType));
	std::vector<double> averagingTimes; averagingTimes.resize(length);

	for (int i = 0; i < length; i++)
	{
		averagingTimes[i] = i * (Time / (length - 1));
	}

	PathDependentAsian theOption(Time, thePayoffF, averagingTimes);
	BasicRandomGenerator theGenerator(121);

	Price = *ExoticMCtwoAssets(theOption, theGenerator);

}

double* ExoticMonteCarlo::ExoticMC(const PathDependentAsian& theOption, RandomGenerator& theGenerator)
{
	double* result = new double[1];


	int numberOfDates = theOption.getAveragingTimes().size();
	double sum = 0.0;

	//outer loop - Paths 
	for (int i = 0; i < numberOfPaths; i++){ 
		double averagePath = 0.0; 
		double lastSpot = Spot; 
		std::vector<double> gaussianVector; gaussianVector.resize(numberOfDates - 1); 

		theGenerator.getStdNormalInverse(gaussianVector); 
		
		//inner loop - sum underlying values on each date 
		
		for (int j = 1; j < numberOfDates; j++)
		{
			double precomputedSpot = lastSpot*exp((Rate - 0.5*Vol*Vol)*(theOption.getAveragingTimes()[j] - theOption.getAveragingTimes()[j - 1])); 
			double stdDeviation = Vol * sqrt(theOption.getAveragingTimes()[j] - theOption.getAveragingTimes()[j - 1]); 
			double thisSpot = precomputedSpot*exp(stdDeviation*gaussianVector[j - 1]); 
			lastSpot = thisSpot; 
			averagePath += thisSpot;
		} 
		
		averagePath = averagePath / (numberOfDates - 1); 
		double payoffCalc = theOption.optionPayoff(averagePath); 
		
		sum += payoffCalc; 
	} 
	
	result[0] = (sum / numberOfPaths) * exp(-Rate * theOption.getTime());

	return result; 
}



double* ExoticMonteCarlo::ExoticMCtwoAssets(const PathDependentAsian& theOption, RandomGenerator& theGenerator)
{
	double* result = new double[1];


	int numberOfDates = theOption.getAveragingTimes().size();
	double sum = 0.0;


	for (int i = 0; i < numberOfPaths; i++)
	{
		double log_sum1 = 0.0;
		double log_sum2 = 0.0;

		double lastSpot1 = Spot;
		double lastSpot2 = Spot2;

		double Epsilon1, Epsilon2;

		std::vector<double> gaussianVector1; gaussianVector1.resize(numberOfDates - 1);
		std::vector<double> gaussianVector2; gaussianVector2.resize(numberOfDates - 1);

		theGenerator.getStdNormalInverse(gaussianVector1);
		theGenerator.getStdNormalInverse(gaussianVector2);

		theGenerator.getStdNormalInverse(gaussianVector1);
		theGenerator.getStdNormalInverse(gaussianVector2);


		for (int j = 1; j < numberOfDates; j++)
		{
			Epsilon1 = gaussianVector1[j - 1];
			Epsilon2 = Rho * Epsilon1 + gaussianVector2[j - 1] * sqrt(1 - Rho*Rho);

			double precomputedSpot1 = lastSpot1*exp((Rate - 0.5*Vol*Vol)*(theOption.getAveragingTimes()[j] - theOption.getAveragingTimes()[j - 1]));
			double stdDeviation1 = Vol * sqrt(theOption.getAveragingTimes()[j] - theOption.getAveragingTimes()[j - 1]);
			double thisSpot1 = precomputedSpot1*exp(stdDeviation1*Epsilon1);
			lastSpot1 = thisSpot1;
			log_sum1 += log(thisSpot1);
			
			double precomputedSpot2 = lastSpot2*exp((Rate - 0.5*Vol2*Vol2)*(theOption.getAveragingTimes()[j] - theOption.getAveragingTimes()[j - 1]));
			double stdDeviation2 = Vol2 * sqrt(theOption.getAveragingTimes()[j] - theOption.getAveragingTimes()[j - 1]);
			double thisSpot2 = precomputedSpot2*exp(stdDeviation2*Epsilon2);
			lastSpot2 = thisSpot2;
			log_sum2 += log(thisSpot2);

		}

		double geom_mean1 = exp(log_sum1 / (numberOfDates-1));
		double geom_mean2 = exp(log_sum2 / (numberOfDates-1));

		double payoffCalc = theOption.optionPayoff(abs(geom_mean1-geom_mean2));

		sum += payoffCalc;

	}

	result[0] = (sum / numberOfPaths) * exp(-Rate * theOption.getTime());

	return result;
}



int ExoticMonteCarlo::getAveradeInt(AverageType AvType)
{
	switch (AvType)
	{
	case DayAverage:
		return 252;

	case WeekAverage:
		return 52;

	case MonthAverage:
		return 12;
	}
}

double ExoticMonteCarlo::getPrice() const
{
	return Price;
}
