#include "AmericanOption.h"
#include "Random.h"
#include <cmath>

AmericanOption::AmericanOption(double Spot_, double Strike_, double Rate_, double DivY_, double Vol_, double Time_, OptionType _type) :
	Spot(Spot_), Strike(Strike_), Time(Time_), Type(_type) {

	Rate = Rate_ / 100.0;
	Vol = Vol_ / 100.0;
	DivYield = DivY_ / 100.0;

	UnderlyingSteps = 100;

	CalculateGrid();
}

void AmericanOption::CalculateGrid() {
	double dt, dS;

	int signTypeOfExp = Type == Call ? 1 : -1;

	SpotPosition = UnderlyingSteps / 2;

	dS = 2 * Spot / UnderlyingSteps;
	deltaX = dS;
	dt = pow(dS / (Vol * 2 * Strike), 2);
	TimeSteps = Time / dt+1;

	Grid.resize(TimeSteps);	

	for (int i = 0; i < TimeSteps; i++) {
		Grid[i].resize(UnderlyingSteps+1);
	}

	for (int i = 0; i <= UnderlyingSteps; i++) {
		SpotArray.push_back(dS*i);
		
	}

	//Grid of Explicite Finite Difference Model
	for (int i = TimeSteps-1; i >= 0; i--)
	{
		for (int j = 0; j <= UnderlyingSteps; j++)
		{
			if (i == TimeSteps - 1)
				Grid[i][j] = fmax(0, signTypeOfExp * (SpotArray[j] - Strike));

			else
			{				
				if (j != 0 && j != (UnderlyingSteps))
				{
					double pu, pm, pd;

					pu = 0.5 * dt * (pow(Vol * j, 2) + (Rate - DivYield) * j);
					pm = 1 - pow(Vol * j, 2) * dt;
					pd = 0.5 * dt * (pow(Vol * j, 2) - (Rate- DivYield) * j);

					Grid[i][j] = fmax((pu * Grid[i + 1][ j + 1] + pm * Grid[i + 1][j] +
						pd * Grid[i + 1][ j - 1]) * 1 / (1 + Rate * dt),
						signTypeOfExp * (SpotArray[j] - Strike));
				}
				else
					Grid[i][j] = Grid[i + 1][j];
			}
		}
	}
}

double AmericanOption::getPrice() const { 
	//Can we build a Grid inside this function? 
	//Find corresponding price using linear interpolation 
	//No. It is const (no change variables)

	double Price;

	Price = Grid[0][SpotPosition - 1]
		+ (Grid[0][SpotPosition] - Grid[0][SpotPosition - 1])
			* (Spot - SpotArray[SpotPosition - 1]) / (SpotArray[SpotPosition]
			- SpotArray[SpotPosition - 1]);

	return Price;

} 

double AmericanOption::getDelta() const{ 
	double resultLeft, resultRight, result; 
	
	//How to calculate derivative with respect to S? 
	// difference next nodes div delta X
	
	resultLeft = (Grid[1][(SpotPosition - 1) + 1] - Grid[1][(SpotPosition - 1) - 1]) 
		/ (2.0 * deltaX); 
	
	resultRight = (Grid[1][SpotPosition + 1] - Grid[1][SpotPosition - 1]) 
		/  (2.0 * deltaX);
	
	result = resultLeft + (resultRight - resultLeft) * (Spot - SpotArray[SpotPosition - 1]) 
		/ (SpotArray[SpotPosition] - SpotArray[SpotPosition - 1]); 
	
	return result; 

}


double AmericanOption::getGamma() const {

	//difference next Deltas div delta X

	double result; 
	
	result = (((Grid[1][SpotPosition + 1] - Grid[1][SpotPosition - 1]) / (2.0 * deltaX))
		- ((Grid[1][(SpotPosition - 1) + 1] - Grid[1][(SpotPosition - 1) - 1]) / (2.0 * deltaX)))
			/ deltaX;
		
	return result; 

} 

//Alternative solution? Disadvantages of the following code double 
//I tryed to find better way, but couldn't. This way is very simple.


double AmericanOption::getVega() const{
	
	double Vega, deltaVol = 0.01; 
	AmericanOption dummy = AmericanOption (Spot, Strike, Rate*100, DivYield*100, (Vol + deltaVol)*100, Time, Type);

	Vega = (dummy.getPrice() - getPrice()) / (deltaVol);
	
	return Vega;
}


double AmericanOption::getTheta() const {

	double Theta, deltaT = 0.01;
	AmericanOption dummy = AmericanOption(Spot, Strike, Rate * 100, DivYield * 100, Vol * 100 , Time - deltaT, Type);

	Theta = (dummy.getPrice() - getPrice()) / (deltaT);

	return Theta;
}

double AmericanOption::getRho() const {

	double Rho, deltaR = 0.01;
	AmericanOption dummy = AmericanOption(Spot, Strike, (Rate + deltaR)* 100, DivYield * 100, Vol * 100, Time, Type);

	Rho = (dummy.getPrice() - getPrice()) / (deltaR);

	return Rho;
}

