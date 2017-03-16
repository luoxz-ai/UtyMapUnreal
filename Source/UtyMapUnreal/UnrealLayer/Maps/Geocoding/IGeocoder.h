#pragma once

#include <string>
#include "GeoCoordinate.h"
#include "BoundingBox.h"

namespace UtyMap { namespace Unreal { namespace Maps { namespace Geocoding { class FGeocoderResult; } } } }

namespace UtyMap
{
	namespace Unreal
	{
		namespace Maps
		{
			namespace Geocoding
			{
				// Defines reverse geocoder API.
				class IGeocoder
				{
				public:
					// Performs geocoding for given place.
					virtual FGeocoderResult* Search(const std::string& name) = 0;

					// Performs geocoding for given place and area.
					virtual FGeocoderResult* Search(const std::string& name, FBoundingBox* area) = 0;

					// Performs reverse geocodin for given geocoordinate.
					virtual FGeocoderResult* Search(FGeoCoordinate* coordinate) = 0;
				};

				// Represents geocoding results.
				class FGeocoderResult
				{
				public:
					// Gets or sets place id.
					long long PlaceId = 0;

					// Gets or sets osm id.
					long long OsmId = 0;

					// Gets or sets osm type.
					std::string OsmType;

					// Gets or sets formatted name of search result.
					std::string DisplayName;

					// Gets or sets class of object.
					std::string Class;

					// Gets or sets type of object.
					std::string Type;

					// Gets or sets geo coordinate of search result center.
					FGeoCoordinate* Coordinate;

					// Gets or sets earch's bounding box.
					FBoundingBox* BoundginBox;
				};
			}
		}
	}
}
