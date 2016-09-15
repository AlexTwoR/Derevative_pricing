#include "PathDependentAsian.h" 
#include "Random.h" 


enum AverageType;
enum OptionType;

class ExoticMonteCarlo {

public:

	double Spot;
	double Spot2;
	double Strike;
	double Rate; //in % annualized 
	double Vol; //in % annualized 
	double Vol2; //in % annualized 
	double Time; //time to maturity in years
	double Price;

	double Rho; //Correlation

	int numberOfPaths;
	OptionType Type;

	ExoticMonteCarlo(double S, double X, double r, double vol, double T, int numberOfPaths, OptionType type, AverageType AvType);
	ExoticMonteCarlo(double S, double S2, double X, double r, double vol, double vol2, double rho, double t, 
		int numberOfPaths, OptionType type, AverageType AvType);

	double getPrice() const;

	double* ExoticMC(const PathDependentAsian& theOption, RandomGenerator& theGenerator);
	double* ExoticMCtwoAssets(const PathDependentAsian& theOption, RandomGenerator& theGenerator);

	double* ExoticMCtwoAssets(const PathDependentAsian& theOption, double Rho,
		RandomGenerator& theGenerator, double Spot, double Vol, double Spot2, double Vol2, double Rate, int numberOfPaths);

private:

	int getAveradeInt(AverageType AvType);
};