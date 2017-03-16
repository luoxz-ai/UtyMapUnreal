#pragma once

#include "IMapDataProvider.h"
#include "IConfigSection.h"
#include "IConfigurable.h"
//#include "IFileSystemService.h"
//#include "INetworkService.h"
//#include "ITrace.h"
#include "Tile.h"
//#include "SimpleJSON.h"
#include "QuadKey.h"
#include <string>
#include <vector>
#include <cmath>
#include <functional>

using namespace UtyDepend;
using namespace UtyDepend::Config;

namespace UtyMap
{
	namespace Unreal
	{
		namespace Maps
		{
			namespace Providers
			{
				class FMapzenElevationDataProvider : public UMapDataProvider, public IConfigurable
				{
				private:
					std::string _mapDataServerUri;
					std::string _mapDataApiKey;
					std::string _elePath;
					int _eleGrid = 0;
					std::string _mapDataFormatExtension;

				public:
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [Dependency] public FMapzenElevationDataProvider(IFileSystemService fileSystemService, INetworkService networkService, ITrace trace) : base(fileSystemService, networkService, trace)
					FMapzenElevationDataProvider(); // IFileSystemService* fileSystemService, INetworkService* networkService, ITrace* trace);

					
					virtual std::string* Get(ATile* tile);

					
				protected:
					void WriteBytes(void* stream, unsigned char bytes[]);

				private:
					std::string GetJsonPayload(FQuadKey* quadkey);

					std::vector<FGeoCoordinate*> CreatePolyline(FQuadKey* quadkey);

					static std::string EncodePolyline(std::vector<FGeoCoordinate*>& points);

					
				public:
					void Configure(IConfigSection* configSection);
				};
			}
		}
	}
}
