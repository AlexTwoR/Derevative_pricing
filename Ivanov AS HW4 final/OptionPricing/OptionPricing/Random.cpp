#include "Random.h" 
#include <cmath> 
 //Using random engine from boost 
#include <boost/math/distributions/inverse_gaussian.hpp> 
#include <boost/random.hpp>

//Available in other code files? 
//No. But if define it in *.h and add #include "..." - yes.
const double PI = 3.141592653589793238463; 

//What is this about? 
//Polynominal Apprtoximations of Normal distribution function

double CDF_PA(double x){ 
    double L = 0.0; 
    double K = 0.0; 
    double dCDF = 0.0; 
    const double a1 = 0.31938153; 
    const double a2 = -0.356563782; 
    const double a3 = 1.781477937; 
    const double a4 = -1.821255978; 
    const double a5 = 1.330274429; 
	L = abs(x); 
    K = 1.0 / (1.0 + 0.2316419 * L); 
    dCDF = 1.0 - 1.0 / sqrt(2 * PI) * exp(-L * L / 2.0) * 
            (a1 * K + a2 * K * K + a3 * pow(K, 3.0) 
            + a4 * pow(K, 4.0) + a5 * pow(K, 5.0)); 
    
    if (x < 0){ 
        return 1.0 - dCDF; 
    }    
    else { 
        return dCDF; 
    } 
}


double CDF_HA (double x){

	double SPLIT = 7.07106781186547;

	const double a0 = 220.206867912376;
	const double a1 = 221.213596169931;
	const double a2 = 112.079291497871;
	const double a3 = 33.912866078383;
	const double a4 = 6.37396220353165;
	const double a5 = 0.700383064443688;
	const double a6 = 0.0352624965998911;
	
	const double b0 = 440.413735824752;
	const double b1 = 793.826512519948;
	const double b2 = 637.333633378831;
	const double b3 = 296.564248779674;
	const double b4 = 86.7807322029461;
	const double b5 = 16.064177579207;
	const double b6 = 1.75566716318264;
	const double b7 = 0.0883883476483184;

	double y = abs(x);
	double CND = 0.0;

	if (y <= 37.0)
	{
		double e = exp(-y*y / 2.0);
		if (y < SPLIT)
		{
			const double sumA = (((((a6*y + a5)*y + a4)*y + a3)*y + a2)*y + a1)*y + a0;
			const double sumB = ((((((b7*y + b6)*y + b5)*y + b4)*y + b3)*y + b2)*y + b1)*y + b0;
			CND = e*sumA / sumB;
		}
		else
		{
			const double sumA = y + 1.0 / (y + 2.0 / (y + 3.0 / (y + 4.0 / (y + 13.0 / 20.0))));
			CND = e / (2.506628274631*sumA);
		}
	}

	return x <= 0.0 ? CND : 1 - CND;
}

double PDF(double x) {

	return exp(-x*x / 2.0) / sqrt(2.0 * PI);

}

double RandGaussian() {
	double x = 0.0;
	double y = 0.0;
	double euclid_sq = 0.0;

	// Continue generating two uniform random variables
	// until the square of their "euclidean distance" 
	// is less than unity
	do {
		x = 2.0 * rand() / static_cast<double>(RAND_MAX) - 1;
		y = 2.0 * rand() / static_cast<double>(RAND_MAX) - 1;
		euclid_sq = x*x + y*y;
	} while (euclid_sq >= 1.0);

	return x*sqrt(-2 * log(euclid_sq) / euclid_sq);
}


void RandomGenerator::getStdNormalInverse(std::vector<double>& arrayToFillGauss) 
{ 
	//Gaussian inverse function from boost 
	boost::math::normal distr(0.0, 1.0); 
	std::vector<double> arrayToFillUni; 
	arrayToFillUni.resize(arrayToFillGauss.size()); 

	//getUniform(arrayToFillUni); 
	getUniformAntiThetic(arrayToFillUni);

	for (int i = 0; i < arrayToFillGauss.size(); i++)
	{
		arrayToFillGauss[i] = quantile(distr, arrayToFillUni[i]);
	} 

	Size = arrayToFillGauss.size(); 
} 


void BasicRandomGenerator::getUniform(std::vector<double>& arrayToFillUni) { 
	//What happens if they are not static? //Uniform engine (Mersenne twister) from boost 
	static boost::mt19937 generator(Seed); 
	static boost::uniform_real<> range(0, 1); 
	static boost::variate_generator<boost::mt19937&, boost::uniform_real<> > uniform(generator, range); 
	for (int i = 0; i < arrayToFillUni.size(); i++)
	{
		arrayToFillUni[i] = uniform();
	} 
}

void BasicRandomGenerator::getUniformAntiThetic(std::vector<double>& arrayToFillUni)
{
	static boost::mt19937 generator(Seed);
	static boost::uniform_real<> range(0, 1);
	static std::vector<double> NextVariates;
	///static boost::variate_generator<boost::mt19937&, boost::uniform_real<> > uniform(generator, range);

	NextVariates.resize(arrayToFillUni.size());	

	if (OddEven)
	{
		getUniform(arrayToFillUni);
		for (unsigned long i = 0; i < arrayToFillUni.size(); i++)
			NextVariates[i] = 1.0 - arrayToFillUni[i];

		OddEven = false;
	}
	else
	{
		arrayToFillUni = NextVariates;
		OddEven = true;
	}
}

RandomGenerator* BasicRandomGenerator::clone() const
{
	return new BasicRandomGenerator(*this);
}

void BasicRandomGenerator::resetSeed()
{
	
}

