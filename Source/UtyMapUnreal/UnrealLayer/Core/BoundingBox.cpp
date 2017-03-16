#include "UtyMapUnreal.h"
#include "BoundingBox.h"
#include "Utils/MathUtils.h"
#include "Utils/GeoUtils.h"

FBoundingBox::FBoundingBox() : FBoundingBox(FGeoCoordinate(90, 180), FGeoCoordinate(-90, -180))
{
}

FBoundingBox::FBoundingBox(FGeoCoordinate minPoint, FGeoCoordinate maxPoint) : MinPoint(minPoint.Latitude, minPoint.Longitude), MaxPoint(maxPoint.Latitude, maxPoint.Longitude)
{
}

void FBoundingBox::Extend(FGeoCoordinate coordinate)
{
	MinPoint = FGeoCoordinate(std::min(MinPoint.Latitude, coordinate.Latitude), std::min(MinPoint.Longitude, coordinate.Longitude));

	MaxPoint = FGeoCoordinate(std::max(MaxPoint.Latitude, coordinate.Latitude), std::max(MaxPoint.Longitude, coordinate.Longitude));
}

FGeoCoordinate FBoundingBox::Center()
{
	return FGeoCoordinate(MinPoint.Latitude + (MaxPoint.Latitude - MinPoint.Latitude) / 2, MinPoint.Longitude + (MaxPoint.Longitude - MinPoint.Longitude) / 2);
}

std::string FBoundingBox::ToString()
{
	return std::string("");
	/*const char* format = "%f,%f";
	auto size = std::snprintf(nullptr, 0, format , MinPoint.ToString(), MaxPoint.ToString() );
	std::string output(size + 1, '\0');
	std::sprintf(&output[0], format, MinPoint.ToString(), MaxPoint.ToString());
	return output;*/
}

FBoundingBox* FBoundingBox::operator + (FGeoCoordinate b)
{
	this->MinPoint = FGeoCoordinate(this->MinPoint.Latitude < b.Latitude ? this->MinPoint.Latitude : b.Latitude, this->MinPoint.Longitude < b.Longitude ? this->MinPoint.Longitude : b.Longitude);

	this->MaxPoint = FGeoCoordinate(this->MaxPoint.Latitude > b.Latitude ? this->MaxPoint.Latitude : b.Latitude, this->MaxPoint.Longitude > b.Longitude ? this->MaxPoint.Longitude : b.Longitude);

	return this;
}

FBoundingBox* FBoundingBox::operator + (const FBoundingBox& b)
{
	auto minLat = this->MinPoint.Latitude < b.MinPoint.Latitude ? this->MinPoint.Latitude : b.MinPoint.Latitude;
	auto minLon = this->MinPoint.Longitude < b.MinPoint.Longitude ? this->MinPoint.Longitude : b.MinPoint.Longitude;

	auto maxLat = this->MaxPoint.Latitude > b.MaxPoint.Latitude ? this->MaxPoint.Latitude : b.MaxPoint.Latitude;
	auto maxLon = this->MaxPoint.Longitude > b.MaxPoint.Longitude ? this->MaxPoint.Longitude : b.MaxPoint.Longitude;

	this->MinPoint = FGeoCoordinate(minLat, minLon);
	this->MaxPoint = FGeoCoordinate(maxLat, maxLon);

	return this;
}

FBoundingBox* FBoundingBox::Create(FGeoCoordinate center, double width, double height)
{
	// Bounding box surrounding the point at given coordinates,
	// assuming local approximation of Earth surface as a sphere
	// of radius given by WGS84
	auto lat = FMathUtils::Deg2Rad(center.Latitude);
	auto lon = FMathUtils::Deg2Rad(center.Longitude);

	// Radius of Earth at given latitude
	auto radius = FGeoUtils::WGS84EarthRadius(lat);
	// Radius of the parallel at given latitude
	auto pradius = radius*std::cos(lat);

	auto dWidth = width / (2*radius);
	auto dHeight = height / (2*pradius);

	auto latMin = lat - dWidth;
	auto latMax = lat + dWidth;
	auto lonMin = lon - dHeight;
	auto lonMax = lon + dHeight;

	return new FBoundingBox(FGeoCoordinate(FMathUtils::Rad2Deg(latMin), FMathUtils::Rad2Deg(lonMin)), FGeoCoordinate(FMathUtils::Rad2Deg(latMax), FMathUtils::Rad2Deg(lonMax)));
}

FBoundingBox* FBoundingBox::Create(FGeoCoordinate center, float sideInMeters)
{
	return Create(center, sideInMeters, sideInMeters);
}

FBoundingBox* FBoundingBox::Empty()
{
	return new FBoundingBox(FGeoCoordinate(std::numeric_limits<double>::max(), std::numeric_limits<double>::max()), FGeoCoordinate(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest()));
}

bool FBoundingBox::Contains(FGeoCoordinate coordinate)
{
	return coordinate.Latitude > MinPoint.Latitude && coordinate.Longitude > MinPoint.Longitude && coordinate.Latitude < MaxPoint.Latitude && coordinate.Longitude < MaxPoint.Longitude;
}

bool FBoundingBox::Intersects(FBoundingBox* bbox)
{
	auto minX = std::max(MinPoint.Latitude, bbox->MinPoint.Latitude);
	auto minY = std::max(MinPoint.Longitude, bbox->MinPoint.Longitude);
	auto maxX = std::min(MaxPoint.Latitude, bbox->MaxPoint.Latitude);
	auto maxY = std::min(MaxPoint.Longitude, bbox->MaxPoint.Longitude);

	return minX <= maxX && minY <= maxY;
}
