#pragma once

#include <string>

#include "GeoCoordinate.generated.h"

/**
 * GeoCoordinate - Latitude and Longitude define them
 */
USTRUCT(BlueprintType)
struct FGeoCoordinate
{
	GENERATED_USTRUCT_BODY()

	double Latitude;
	double Longitude;

	// Latitude with float precision for display in editor
	UPROPERTY(EditAnywhere)
	float LatitudeFloat;

	// Longitude with float precision for display in editor
	UPROPERTY(EditAnywhere)
	float LongitudeFloat;

	FGeoCoordinate(double latitude = 0.0, double longitude = 0.0);

	// Compares two geo coordinates.
	bool operator == (FGeoCoordinate b);

	// Compares two geo coordinates.
	bool operator != (FGeoCoordinate b);

	FGeoCoordinate operator=(const FGeoCoordinate& b);
	virtual bool Equals(void* other);
	virtual int GetHashCode();
	virtual std::string ToString();
};