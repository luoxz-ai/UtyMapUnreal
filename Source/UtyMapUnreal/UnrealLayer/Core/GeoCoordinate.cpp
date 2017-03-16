#include "UtyMapUnreal.h"
#include "GeoCoordinate.h"
#include "Utils/MathUtils.h"
#include "Utils/GeoUtils.h"
#include <sstream>

FGeoCoordinate::FGeoCoordinate(double latitude, double longitude) : Latitude(latitude), Longitude(longitude)
{
	LatitudeFloat = Latitude;
	LongitudeFloat = Longitude;
}

bool FGeoCoordinate::operator == (FGeoCoordinate b)
{
	return FMathUtils::AreEqual(this->Latitude, b.Latitude) && FMathUtils::AreEqual(this->Longitude, b.Longitude);
}

bool FGeoCoordinate::operator != (FGeoCoordinate b)
{
	return !(FMathUtils::AreEqual(this->Latitude, b.Latitude) && FMathUtils::AreEqual(this->Longitude, b.Longitude));
}

FGeoCoordinate FGeoCoordinate::operator=(const FGeoCoordinate& b)
{
	return FGeoCoordinate(b.Latitude, b.Longitude);
}

bool FGeoCoordinate::Equals(void* other)
{
	//if (!(dynamic_cast<FGeoCoordinate>(other) != nullptr))
	if (other == nullptr)
		return false;
	auto coord = static_cast<FGeoCoordinate*>(other);
	return FMathUtils::AreEqual(Latitude, coord->Latitude) && FMathUtils::AreEqual(Longitude, coord->Longitude);
}

int FGeoCoordinate::GetHashCode()
{
	return 0;
	// TODO: implement
	//return static_cast<int>(Latitude ^ Longitude << 2);
	//return Latitude.GetHashCode() ^ Longitude.GetHashCode() << 2;
}

std::string FGeoCoordinate::ToString()
{
	std::stringstream ss;
	ss << "GeoCoordinate(" << "Lat:" << Latitude << ", Lon:" << Longitude;
	return ss.str();
}