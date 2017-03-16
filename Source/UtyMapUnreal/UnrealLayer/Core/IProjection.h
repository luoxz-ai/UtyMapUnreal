#pragma once

#include "NoExportTypes.h"
#include "GeoCoordinate.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include "IProjection.generated.h"

// Projects GeoCoordinate to 2D plane.
UCLASS()
class UCartesianProjection : public UObject
{
	GENERATED_BODY()

private:
	FGeoCoordinate _worldZeroPoint;
	double Scale = 0.01; // Unreal units [cm] to Meter scale. could be read from engine (GetWorld()->WorldScaleToMeters())

public:
	// Creates instance of CartesianProjection
	// @param worldZeroPoint - GeoCoordinate for (0, 0) point
	UCartesianProjection(FGeoCoordinate worldZeroPoint);
	UCartesianProjection(const FObjectInitializer& ObjectInitializer);

	void Init(FGeoCoordinate worldZeroPoint);
	FVector* Project(FGeoCoordinate coordinate, double height);
	FGeoCoordinate Project(FVector* worldCoordinate);
	std::string GetWorldZeroPoint() { return _worldZeroPoint.ToString(); }
};

// Projects GeoCoordinate to sphere.
class FSphericalProjection // : public UCartesianProjection
{
private:
	const float _radius;

public:
	// Creates instance of SphericalProjection
	// @param radius Radius of sphere.
	FSphericalProjection(float radius);

	FVector* Project(FGeoCoordinate coordinate, double height);
	FGeoCoordinate Project(FVector* worldCoordinate);
};
