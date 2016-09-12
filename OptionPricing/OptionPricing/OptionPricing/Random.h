#ifndef RANDOM_H 
#define RANDOM_H 

//Cumulative Normal distribution function 
double CDF_PA(double); //Polynominal Apprtoximations
double CDF_HA(double); //The Hart Algorithm

//Probability density function 
double PDF(double); 

double RandGaussian();

#endif

