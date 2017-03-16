#pragma once

#include "GeoCoordinate.h"
#include <string>
#include <cmath>
#include <limits>



// Represents bounding box.
// See details:
// http://stackoverflow.com/questions/238260/how-to-calculate-the-bounding-box-for-a-given-lat-lng-location
///
class FBoundingBox
{
public:
	// Point with maximum latitude and longitude.
	FGeoCoordinate MaxPoint;

	// Point with minimal latitude and longitude.
	FGeoCoordinate MinPoint;

	// Creates invalid bounding box which can be modified later (e.g. by calling Extend).
	FBoundingBox();

	// Creates bounding box from given min and max points.
	// @param minPoint - Point with minimal latitude and longitude
	// @param maxPoint - Point with maximum latitude and longitude
	FBoundingBox(FGeoCoordinate minPoint, FGeoCoordinate maxPoint);

	// Extends current bounding box by given coordinate.
	void Extend(FGeoCoordinate coordinate);

	FGeoCoordinate Center();
	
	virtual std::string ToString();

	// Adds geo coordinate to a bounding box.
	FBoundingBox* operator + (FGeoCoordinate b);

	// Adds the right bounding box to the left one.
	FBoundingBox* operator + (const FBoundingBox& b);

	// Creates bounding box as rectangle.
	// @param center - Center point
	// @param width - Width in meters
	// @param height - Heigh in meters
	static FBoundingBox* Create(FGeoCoordinate center, double width, double height);

	// Creates bounding box as square.
	// @param center - Center
	// @param sideInMeters - Length of the bounding box
	static FBoundingBox* Create(FGeoCoordinate center, float sideInMeters);

	// Creates empty bounding box.
	static FBoundingBox* Empty();

	// Checks whether given coordinate inside bounding box.
	bool Contains(FGeoCoordinate coordinate);

	// Checks whether bounding boxes are intersects
	bool Intersects(FBoundingBox* bbox);
};
