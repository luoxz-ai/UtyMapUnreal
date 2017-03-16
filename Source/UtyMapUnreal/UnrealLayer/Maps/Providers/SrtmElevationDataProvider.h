#pragma once

#include "IMapDataProvider.h"
#include "IConfigurable.h"
#include "Tile.h"
#include "IConfigSection.h"
#include "GeoCoordinate.h"
#include <string>
#include <unordered_map>
#include <cmath>
#include <mutex>

using namespace UtyDepend::Config;

namespace UtyMap
{
	namespace Unreal
	{
		namespace Maps
		{
			namespace Providers
			{
				// Downloads SRTM data from NASA server.
				class FSrtmElevationDataProvider : public UMapDataProvider, public IConfigurable
				{
				private:
					static const std::string TraceCategory;
					std::mutex _lock;

					//IFileSystemService* const  _fileSystemService;
					//INetworkService* const  _networkService;
					//ITrace* const  _trace;
					std::string _server;
					std::string _schemaPath;
					std::string _elePath;

					static const std::unordered_map<int, std::string> ContinentMap;

					// Creates instance of <see cref="SrtmElevationDataProvider"/>.
				public:
					FSrtmElevationDataProvider(); // IFileSystemService* fileSystemService, INetworkService* networkService, ITrace* trace);


					
					std::string* Get(ATile* tile);

					
					void Configure(IConfigSection* configSection);


				private:
					std::string GetFileNamePrefix(FGeoCoordinate* coordinate);

					// Download SRTM data for given coordinate.
					//IObservable<unsigned char[]>* Download(const std::string& prefix);
				};
			}
		}
	}
}
