#pragma once

#include "IGeocoder.h"
#include "IConfigSection.h"
#include "IConfigurable.h"
//#include "INetworkService.h"
#include "GeoCoordinate.h"
#include "BoundingBox.h"
//#include "SimpleJSON.h"
#include <string>
#include <unordered_map>
#include "stringbuilder.h"


namespace UtyMap { namespace Unreal { namespace Maps { namespace Geocoding { class FGeocoderResult; } } } }

using namespace UtyDepend;
using namespace UtyDepend::Config;

namespace UtyMap
{
	namespace Unreal
	{
		namespace Maps
		{
			namespace Geocoding
			{
				// Geocoder which uses osm nominatim.
				class NominatimGeocoder : public IGeocoder, public IConfigurable
				{

				private:
					static const std::string DefaultSearchServer;
					static const std::string DefaultReverseSearchServer;

					// NOTE: nominatim wants user agent header.
					const std::unordered_map<std::string, std::string> _headers = std::unordered_map<std::string, std::string>
					{
						{"User-Agent", "UtyMap"}
					};

					//INetworkService* const  _networkService;

					std::string _searchPath;
					std::string _reverseSearchPath;

				public:
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [Dependency] public NominatimGeocoder(INetworkService networkService)
					NominatimGeocoder(); // INetworkService* networkService);

					
					FGeocoderResult* Search(const std::string& name);

					
					FGeocoderResult* Search(const std::string& name, FBoundingBox* area);

					
					FGeocoderResult* Search(FGeoCoordinate* coordinate);

				private:
					//FGeocoderResult* ParseGeocoderResult(FJSONNode* resultNode);

					static FGeoCoordinate* ParseGeoCoordinate(const std::string& latStr, const std::string& lonStr);

					
				public:
					void Configure(IConfigSection* configSection);
				};
			}
		}
	}
}
