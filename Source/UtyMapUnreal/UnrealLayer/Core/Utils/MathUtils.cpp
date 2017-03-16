#include "UtyMapUnreal.h"
#include "MathUtils.h"


double FMathUtils::Deg2Rad(double degrees)
{
	return M_PI*degrees / 180.0;
}

double FMathUtils::Rad2Deg(double radians)
{
	return 180.0*radians / M_PI;
}

bool FMathUtils::AreEqual(double a, double b, double epsilon)
{
	return std::abs(a - b) < epsilon;
}

double FMathUtils::Clamp(double value, double min, double max)
{
	return (value < min) ? min : (value > max) ? max : value;
}
			
