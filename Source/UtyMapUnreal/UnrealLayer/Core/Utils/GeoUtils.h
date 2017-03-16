#pragma once

#include "NoExportTypes.h"
#include "GeoCoordinate.h"
#include "BoundingBox.h"
#include "QuadKey.h"
#include "IProjection.h"
#include "Rectangle.h"
#define _USE_MATH_DEFINES
#include <vector>
#include <cmath>



// Provides the methods to convert geo coordinates to map coordinates and vice versa.
class FGeoUtils
{

	// The circumference at the equator (latitude 0).
private:
	static constexpr int LatitudeEquator = 40075160;

	// Distance of full circle around the earth through the poles.
	static constexpr int CircleDistance = 40008000;

	
	// Calculates map coordinate from geo coordinate
	// see http://stackoverflow.com/questions/3024404/transform-longitude-latitude-into-meters?rq=1
	///
public:
	static FVector2D* ToMapCoordinate(FGeoCoordinate relativeNullPoint, FGeoCoordinate coordinate);

	// Calculates geo coordinate from map coordinate.
	static FGeoCoordinate ToGeoCoordinate(FGeoCoordinate relativeNullPoint, FVector2D* mapPoint);

	// Calculates geo coordinate from map coordinate.
	static FGeoCoordinate ToGeoCoordinate(FGeoCoordinate relativeNullPoint, double x, double y);



	// Semi-axes of WGS-84 geoidal reference
private:
	static constexpr double WGS84_a = 6378137.0; // Major semiaxis [m]
	static constexpr double WGS84_b = 6356752.3; // Minor semiaxis [m]

	// Calculates distance between two coordinates.
	// @param first">First coordinate
	// @param second">Second coordinate
	// @return Distance
public:
	static double Distance(FGeoCoordinate first, FGeoCoordinate second);

	// Earth radius at a given latitude, according to the WGS-84 ellipsoid [m].
	static double WGS84EarthRadius(double lat);



	static FQuadKey CreateQuadKey(FGeoCoordinate coordinate, int levelOfDetail);

	static FBoundingBox* QuadKeyToBoundingBox(FQuadKey quadKey);

	static FRectangle* QuadKeyToRect(UCartesianProjection* projection, FQuadKey quadKey);

	static FBoundingBox* RectToBoundingBox(UCartesianProjection* projection, FRectangle* rectangle);

	static std::vector<FQuadKey> BoundingBoxToQuadKeys(FBoundingBox* bbox, int levelOfDetails);

private:
	static int LonToTileX(double longitude, int levelOfDetail);

	static int LatToTileY(double latitude, int levelOfDetail);

		static double TileXToLon(int x, int levelOfDetail);

	static double TileYToLat(int y, int levelOfDetail);

	static double Clip(double n, double minValue, double maxValue);

};
