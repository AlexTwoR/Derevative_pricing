#include "BarrierOption.h"
#include <cmath>
#include <vector>


AmericanBarrier::AmericanBarrier(double Spot_, double Strike_, double Rate_, double Vol_,
	double Time_, double DividendYield_, double Barrier_, OptionType type_) 
	: Spot(Spot_), Strike(Strike_), Time(Time_), Barrier(Barrier_), Type(type_) 
{
	Rate = Rate_ / 100.0;
	Vol = Vol_ / 100.0;
	
	UnderlyingSteps = 50; //Pay attention to the number of steps 
	TimeSteps = 30;
	DividendYield = DividendYield_ / 100.0;

	CalculateGrid();

}


void AmericanBarrier::CalculateGrid() {

	if (Type == Call && Barrier > Strike || Type == Put && Barrier < Strike) {
		//define vars
		std::vector<double> Maturity;
		std::vector<double> BufMatrix;

		int signTypeOfExp = Type == Call ? 1 : -1;
		int numberGrid;

		double s_max = 2 * Spot;
		double s_min = 0;


		if (Type == Call) {
			s_max = Barrier;
		}
		else {
			s_min = Barrier;
		}

		numberGrid = UnderlyingSteps + UnderlyingSteps % 2;
		dS = abs(s_max - s_min) / numberGrid;
		dt = Time / TimeSteps;


		SpotPosition = Spot / (s_max - s_min)*numberGrid; //numberGrid / 2;

		//define arries
		bool spotFinding = false;
		for (int i = 0; i < numberGrid + 1; i++) {
			SpotArray.push_back(s_min + dS*i);
			if (SpotArray[i]>=Spot && !spotFinding)
			{
				SpotPosition = i;
				spotFinding = true;
			}
		}

		Grid.resize(numberGrid + 1);
		Maturity.resize(numberGrid + 1);
		BufMatrix.resize(numberGrid + 1);

		for (int i = 0; i < numberGrid + 1; i++) {
			Grid[i].resize(numberGrid + 1);
		}

		for (int i = 0; i < numberGrid + 1; i++)
		{
			BufMatrix[i] = 0;
			Maturity[i] = fmax(0, signTypeOfExp * (SpotArray[i] - Strike)); //define maturity value
			for (int j = 0; j < numberGrid; j++)
				Grid[i][j] = 0; //fill matrix
		}

		//define a, b, c vectors
		Grid[0][0] = 1;
		for (int i = 1; i < numberGrid; i++)
		{
			Grid[i][i - 1] = 0.5*i*((Rate - DividendYield) - Vol*Vol*i)*dt;
			Grid[i][i] = 1 + (Rate + Vol*Vol * i*i)*dt;
			Grid[i][i + 1] = 0.5*i*(-(Rate - DividendYield) - Vol*Vol*i)*dt;
		}
		Grid[numberGrid][numberGrid] = 1;

		Grid = inverse(Grid);

		for (int i = 1; i < numberGrid-1; i++)
		{
			for (int j = 0; j < numberGrid + 1; j++)
				BufMatrix[i] += Maturity[j] * Grid[i][j];
		}


		for (int t = 1; t < TimeSteps; t++)
		{

			Maturity = BufMatrix;

			for (int i = 0; i < numberGrid; i++)
			{
				BufMatrix[i] = 0;
				for (int j = 0; j < numberGrid + 1; j++)
					BufMatrix[i] += Maturity[j] * Grid[i][j];
			}

			for (int k = 0; k < numberGrid + 1; k++)
				BufMatrix[k] = fmax(BufMatrix[k], signTypeOfExp * (SpotArray[k] - Strike));

		}

		Price = BufMatrix[SpotPosition];
	}
	else
		Price = 0;

}


double AmericanBarrier::getPrice() const
{
	return Price;
}


double AmericanBarrier::getDelta() const
{

	double Delta, deltaSpot=0.1;
	AmericanBarrier dummy = AmericanBarrier(Spot+deltaSpot, Strike, Rate * 100, (Vol) * 100, Time, DivYield * 100, Barrier, Type);

	Delta = (dummy.getPrice() - getPrice()) / (deltaSpot);

	return Delta;
}

double AmericanBarrier::getGamma() const
{
	if (Price == 0)
		return 0;
	
	double result;

	result = (((Grid[1][SpotPosition + 1] - Grid[1][SpotPosition - 1]) / (2.0 * dt))
		- ((Grid[1][(SpotPosition - 1) + 1] - Grid[1][(SpotPosition - 1) - 1]) / (2.0 * dt)))
		/ dt;

	return result;
}

double AmericanBarrier::getTheta() const
{
	double Theta, deltaT = 0.01;
	AmericanBarrier dummy = AmericanBarrier(Spot, Strike, Rate * 100, (Vol)* 100, Time- deltaT, DivYield * 100, Barrier, Type);

	Theta = (dummy.getPrice() - getPrice()) / (deltaT);

	return Theta;
}

double AmericanBarrier::getRho() const
{
	double Rho, deltaR = 0.01;
	AmericanBarrier dummy = AmericanBarrier(Spot, Strike, (Rate + deltaR) * 100, (Vol) * 100, Time, DivYield * 100, Barrier, Type);

	Rho = (dummy.getPrice() - getPrice()) / (deltaR);

	return Rho;
}

double AmericanBarrier::getVega() const
{
	double Vega, deltaVol = 0.01;
	AmericanBarrier dummy = AmericanBarrier(Spot, Strike, Rate * 100,  (Vol + deltaVol) * 100, Time, DivYield * 100, Barrier,Type);

	Vega = (dummy.getPrice() - getPrice()) / (deltaVol);

	return Vega;
}


std::vector <std::vector <double> > AmericanBarrier::inverse(std::vector <std::vector <double> > a) const
{
	int n = a.size();
	std::vector <std::vector <double> > ans(n, std::vector <double>(n, 0));
	for (int i = 0; i < n; i++) {
		ans[i][i] = 1.0;
	}
	for (int i = 0; i < n; i++) {
		int row = i;
		double mx = a[i][i];
		for (int k = i + 1; k < n; k++) {
			if (abs(a[k][i]) > mx) {
				row = k;
				mx = abs(a[k][i]);
			}
		}
		if (row != i) {
			std::swap(a[row], a[i]);
			std::swap(ans[row], ans[i]);
		}
		for (int j = i + 1; j < n; j++) {
			double e = a[j][i] / a[i][i];
			for (int k = 0; k < n; k++) {
				a[j][k] -= e*a[i][k];
				ans[j][k] -= e*ans[i][k];
			}
		}
	}
	for (int i = n - 1; i >= 0; i--) {
		for (int j = i - 1; j >= 0; j--) {
			double e = a[j][i] / a[i][i];
			for (int k = 0; k < n; k++) {
				a[j][k] -= e*a[i][k];
				ans[j][k] -= e*ans[i][k];
			}
		}
		for (int j = 0; j < n; j++) {
			ans[i][j] /= a[i][i];
		}
	}
	return ans;
}
