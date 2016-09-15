#ifndef OPTIONPROP_H 
#define OPTIONPROP_H 


enum OptionType
{
	Call,
	Put
};

enum OptionStyle
{
	European,
	American,
	Bermudan,
	Barrier,
	Digital,
	Asian

};



enum AverageType
{
	DayAverage,
	WeekAverage,
	MonthAverage

};


#endif