#pragma once

#include "GeoCoordinate.h"


// Represent geo position which is produced by location services (e.g. GLONASS, GPS).
class FGeoPosition
{
	// Geo coordinate.
public:
	FGeoCoordinate Coordinate;

	// Time.
	// Todo: implement
	//FTimeSpan* Time = FTimeSpan::MinValue;
};