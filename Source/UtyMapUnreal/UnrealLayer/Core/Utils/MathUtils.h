#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

class FMathUtils
{
	// Round digit count.
public:
	static constexpr int RoundDigitCount = 5;

	// Current epsion
	static constexpr double Epsion = 0.00001;

	// Converts degrees to radians.
	static double Deg2Rad(double degrees);

	// Converts radians to degrees.
	static double Rad2Deg(double radians);

	// Compares equality of two double using epsilon.
	// @param a">First double
	// @param b">Second double
	// @param epsilon">Epsilon
	// @return True if equal
	static bool AreEqual(double a, double b, double epsilon = 0.001);

	// Clamp function.
	static double Clamp(double value, double min, double max);
};
		
