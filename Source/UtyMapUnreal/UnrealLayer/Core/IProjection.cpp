#include "UtyMapUnreal.h"
#include "IProjection.h"
#include "Utils/GeoUtils.h"

UCartesianProjection::UCartesianProjection(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), Scale(0.01)
{
}

UCartesianProjection::UCartesianProjection(FGeoCoordinate worldZeroPoint) : _worldZeroPoint(worldZeroPoint), Scale(0.01)
{
}

void UCartesianProjection::Init(FGeoCoordinate worldZeroPoint)
{
	//_worldZeroPoint = worldZeroPoint; // doesn't work, I don't get it!
	_worldZeroPoint.Latitude = worldZeroPoint.Latitude;
	_worldZeroPoint.LatitudeFloat = worldZeroPoint.LatitudeFloat;
	_worldZeroPoint.Longitude = worldZeroPoint.Longitude;
	_worldZeroPoint.LongitudeFloat = worldZeroPoint.LongitudeFloat;
}

// converts to unreal coordinates: x is up, y is right, z is up and scale is 100 [cm] to 1 [m]
FVector* UCartesianProjection::Project(FGeoCoordinate coordinate, double height)
{
	auto point = FGeoUtils::ToMapCoordinate(_worldZeroPoint, coordinate);
	return new FVector((1 / Scale) * point->Y, (1 / Scale) * point->X, (1 / Scale) * static_cast<float>(height));
}

FGeoCoordinate UCartesianProjection::Project(FVector* worldCoordinate)
{
	FVector2D tempVar(worldCoordinate->Y * Scale, worldCoordinate->X * Scale);
	return FGeoUtils::ToGeoCoordinate(_worldZeroPoint, &tempVar);
}

FSphericalProjection::FSphericalProjection(float radius) : _radius(radius)
{
}

FVector* FSphericalProjection::Project(FGeoCoordinate coordinate, double height)
{
	// the x-axis goes through long,lat (0,0), so longitude 0 meets the equator;
	// the y-axis goes through (0,90);
	// and the z-axis goes through the pol
	// x = R * cos(lat) * cos(lon)
	// y = R * cos(lat) * sin(lon)
	// z = R * sin(lat)

	double radius = _radius + height;
	double latRad = (M_PI / 180) * coordinate.Latitude;
	double lonRad = (M_PI / 180) * coordinate.Longitude;
	float x = static_cast<float>(radius * std::cos(latRad) * std::cos(lonRad));
	float y = static_cast<float>(radius * std::cos(latRad) * std::sin(lonRad));
	float z = static_cast<float>(radius * std::sin(latRad));

	return new FVector(x, y, z);
}

FGeoCoordinate FSphericalProjection::Project(FVector* worldCoordinate)
{
	return FGeoCoordinate(0, 0);
	//throw NotImplementedException();
}
