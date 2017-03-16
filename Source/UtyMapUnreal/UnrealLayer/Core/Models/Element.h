#pragma once

#include <string>
#include <unordered_map>
#include "GeoCoordinate.h"
#include "Element.generated.h"

/**
* OSM specific data reprsentation.
*/
USTRUCT(BlueprintType, Blueprintable)
struct FElement
{
	GENERATED_USTRUCT_BODY()

public:
	long long Id;
	std::vector<FGeoCoordinate> Geometry;
	std::vector<double> Heights;
	std::unordered_map<std::string, std::string> Tags;
	std::unordered_map<std::string, std::string> Styles;
/*
	FElement(long long id, std::vector<FGeoCoordinate>& geometry, std::vector<double>& heights, std::unordered_map<std::string, std::string>& tags, std::unordered_map<std::string, std::string>& styles);*/
};	