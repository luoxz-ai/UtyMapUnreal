#pragma once

#include "IMapDataProvider.h"
#include "IConfigurable.h"
//#include "IFileSystemService.h"
//#include "INetworkService.h"
//#include "ITrace.h"
#include "Tile.h"
#include "IConfigSection.h"
#include <string>

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
				// Downloads map data from mapzen servers.
				class FMapzenMapDataProvider : public UMapDataProvider, public IConfigurable
				{
				public:
					//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
					//ORIGINAL LINE: [Dependency] public FMapzenMapDataProvider(IFileSystemService fileSystemService, INetworkService networkService, ITrace trace) : base(fileSystemService, networkService, trace)
					FMapzenMapDataProvider(); // IFileSystemService* fileSystemService, INetworkService* networkService, ITrace* trace);

					virtual std::string* Get(ATile* tile);
					void Configure(IConfigSection* configSection);

				private:
					std::string _cachePath;
					std::string _mapDataServerUri;
					std::string _mapDataFormatExtension;
					std::string _mapDataLayers;
					std::string _mapDataApiKey;
				};
			}
		}
	}
}
