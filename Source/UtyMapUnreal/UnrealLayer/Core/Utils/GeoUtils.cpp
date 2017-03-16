#include "UtyMapUnreal.h"
#include "GeoUtils.h"
#include "MathUtils.h"



FVector2D* FGeoUtils::ToMapCoordinate(FGeoCoordinate relativeNullPoint, FGeoCoordinate coordinate)
{
	double deltaLatitude = coordinate.Latitude - relativeNullPoint.Latitude;
	double deltaLongitude = coordinate.Longitude - relativeNullPoint.Longitude;
	double latitudeCircumference = LatitudeEquator*std::cos(FMathUtils::Deg2Rad(relativeNullPoint.Latitude));
	double resultX = deltaLongitude*latitudeCircumference / 360;
	double resultY = deltaLatitude*CircleDistance / 360;

	return new FVector2D(static_cast<float>(resultX), static_cast<float>(resultY));
}

FGeoCoordinate FGeoUtils::ToGeoCoordinate(FGeoCoordinate relativeNullPoint, FVector2D* mapPoint)
{
	return ToGeoCoordinate(relativeNullPoint, mapPoint->X, mapPoint->Y);
}

FGeoCoordinate FGeoUtils::ToGeoCoordinate(FGeoCoordinate relativeNullPoint, double x, double y)
{
	double latitudeCircumference = LatitudeEquator*std::cos(FMathUtils::Deg2Rad(relativeNullPoint.Latitude));

	auto deltaLongitude = (x*360) / latitudeCircumference;
	auto deltaLatitude = (y*360) / CircleDistance;

	return FGeoCoordinate(relativeNullPoint.Latitude + deltaLatitude, relativeNullPoint.Longitude + deltaLongitude);
}

double FGeoUtils::Distance(FGeoCoordinate first, FGeoCoordinate second)
{
	auto dLat = FMathUtils::Deg2Rad((first.Latitude - second.Latitude));
	auto dLon = FMathUtils::Deg2Rad((first.Longitude - second.Longitude));

	auto lat1 = FMathUtils::Deg2Rad(first.Latitude);
	auto lat2 = FMathUtils::Deg2Rad(second.Latitude);

	auto a = std::sin(dLat / 2) * std::sin(dLat / 2) + std::sin(dLon / 2) * std::sin(dLon / 2) * std::cos(lat1) * std::cos(lat2);

	auto c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

	auto radius = WGS84EarthRadius(dLat);

	return radius * c;
}

double FGeoUtils::WGS84EarthRadius(double lat)
{
	// http://en.wikipedia.org/wiki/Earth_radius
	auto an = WGS84_a * WGS84_a * std::cos(lat);
	auto bn = WGS84_b * WGS84_b * std::sin(lat);
	auto ad = WGS84_a * std::cos(lat);
	auto bd = WGS84_b * std::sin(lat);
	return std::sqrt((an * an + bn * bn) / (ad * ad + bd * bd));
}

FQuadKey FGeoUtils::CreateQuadKey(FGeoCoordinate coordinate, int levelOfDetail)
{
	return FQuadKey(LonToTileX(Clip(coordinate.Longitude, -179.9999999, 179.9999999), levelOfDetail), LatToTileY(Clip(coordinate.Latitude, -85.05112877, 85.05112877), levelOfDetail), levelOfDetail);
}

FBoundingBox* FGeoUtils::QuadKeyToBoundingBox(FQuadKey quadKey)
{
	int levelOfDetail = quadKey.LevelOfDetail;

	auto minPoint = FGeoCoordinate(TileYToLat(quadKey.TileY + 1, levelOfDetail), TileXToLon(quadKey.TileX, levelOfDetail));

	auto maxPoint = FGeoCoordinate(TileYToLat(quadKey.TileY, levelOfDetail), TileXToLon(quadKey.TileX + 1, levelOfDetail));

	return new FBoundingBox(minPoint, maxPoint);
}

FRectangle* FGeoUtils::QuadKeyToRect(UCartesianProjection* projection, FQuadKey quadKey)
{
	auto boundingBox = QuadKeyToBoundingBox(quadKey);
	auto minPoint = projection->Project(boundingBox->MinPoint, 0);
	auto maxPoint = projection->Project(boundingBox->MaxPoint, 0);

	// Todo: make sure coordinates x,y,z are mapped correctly
	return new FRectangle(minPoint->X, minPoint->Y, maxPoint->X - minPoint->X, maxPoint->Y - minPoint->Y);
}

FBoundingBox* FGeoUtils::RectToBoundingBox(UCartesianProjection* projection, FRectangle* rectangle)
{
	FVector tempVar(rectangle->GetBottomLeft()->X, 0, rectangle->GetBottomLeft()->Y);
	auto minPoint = projection->Project(&tempVar);
	FVector tempVar2(rectangle->GetTopRight()->X, 0, rectangle->GetTopRight()->Y);
	auto maxPoint = projection->Project(&tempVar2);

	return new FBoundingBox(minPoint, maxPoint);
}

std::vector<FQuadKey> FGeoUtils::BoundingBoxToQuadKeys(FBoundingBox* bbox, int levelOfDetails)
{
	std::vector<FQuadKey> FoundQuadKeys;
	auto minQuadKey = CreateQuadKey(bbox->MinPoint, levelOfDetails);
	auto maxQuadKey = CreateQuadKey(bbox->MaxPoint, levelOfDetails);

	auto width = maxQuadKey.TileX - minQuadKey.TileX + 1;
	auto height = minQuadKey.TileY - maxQuadKey.TileY + 1;

	auto halfWidth = width / 2;
	auto halfHeight = height / 2;

	auto xBase = minQuadKey.TileX + halfWidth;
	auto yBase = maxQuadKey.TileY + halfHeight;

	int x = 0, y = 0, dx = 0, dy = -1;
	int t = std::max(width, height);
	int maxI = t*t;
	for (int i = 0; i < maxI; i++)
	{
		if ((-halfWidth <= x) && (x <= halfWidth) && (-halfHeight <= y) && (y <= halfHeight))
		{
//C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
			auto FoundQuadKey = new FQuadKey(xBase + x, yBase - y, levelOfDetails);
			FoundQuadKeys.push_back(*FoundQuadKey);
		}

		if ((x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1 - y)))
		{
			t = dx;
			dx = -dy;
			dy = t;
		}
		x += dx;
		y += dy;
	}
	return FoundQuadKeys;
}

int FGeoUtils::LonToTileX(double longitude, int levelOfDetail)
{
	return static_cast<int>(std::floor((longitude + 180.0) / 360.0 * std::pow(2.0, levelOfDetail)));
}

int FGeoUtils::LatToTileY(double latitude, int levelOfDetail)
{
	return static_cast<int>(std::floor((1.0 - std::log(std::tan(latitude * M_PI / 180.0) + 1.0 / std::cos(latitude * M_PI / 180.0)) / M_PI) / 2.0 * std::pow(2.0, levelOfDetail)));
}

double FGeoUtils::TileXToLon(int x, int levelOfDetail)
{
					return x / std::pow(2.0, levelOfDetail) * 360.0 - 180;
}

double FGeoUtils::TileYToLat(int y, int levelOfDetail)
{
	double n = M_PI - 2.0 * M_PI * y / std::pow(2.0, levelOfDetail);
	return 180.0 / M_PI * std::atan(0.5 * (std::exp(n) - std::exp(-n)));
}

double FGeoUtils::Clip(double n, double minValue, double maxValue)
{
	return std::max(minValue, std::min(n, maxValue));
}
