#include "Bridge.h"

Bridge::Bridge(const Payoff& initialPayoff)
{
	thePayoff = initialPayoff.clone();
}

Bridge::Bridge(const Bridge& initial)
{
	thePayoff = initial.thePayoff->clone();
}
double Bridge::operator()(double Spot) const
{
	return thePayoff->operator ()(Spot);
}