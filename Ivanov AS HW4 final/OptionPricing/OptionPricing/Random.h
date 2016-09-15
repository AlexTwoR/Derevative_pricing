#ifndef RANDOM_H 
#define RANDOM_H 
#include <vector>

//Cumulative Normal distribution function 
double CDF_PA(double); //Polynominal Apprtoximations
double CDF_HA(double); //The Hart Algorithm

//Probability density function 
double PDF(double); 

double RandGaussian();


//Abstract class - define the interface, not all member functions are pure virtual 

class RandomGenerator{ 
public: 
	//Simulate array of standard normal variables 
	virtual void getStdNormalInverse(std::vector<double>& arrayToFillGauss); //Simulate array of uniform variables 
	virtual void getUniform(std::vector<double>& arrayToFillUni) = 0; 
	virtual void getUniformAntiThetic(std::vector<double>& arrayToFillUni) = 0;
	int getSize() const
	{ return Size; } 
	void setSize(int newSize) { Size = newSize;} 
	virtual RandomGenerator* clone() const = 0; 
	RandomGenerator(unsigned long Size_) : Size(Size_) {}

private: 
	//Dimension of the array 
	int Size; 
	
}; 


class BasicRandomGenerator : public RandomGenerator{ 
public: 
	virtual void getUniform(std::vector<double>& arrayToFillUni); 
	virtual void getUniformAntiThetic(std::vector<double>& arrayToFillUni);
	BasicRandomGenerator(unsigned long Seed_, int Size_ = 1) : RandomGenerator(Size_), Seed(Seed_){} 
	virtual RandomGenerator* clone() const; 
	void resetSeed(); 

private: //Seed of the random generator unsigned 
	long Seed;
	bool OddEven = true;
};




#endif

