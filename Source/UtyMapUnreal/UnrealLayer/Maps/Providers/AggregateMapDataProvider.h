#pragma once

#include "IMapDataProvider.h"
#include "IConfigurable.h"
#include <string>
#include "ElevationDataType.h"
#include "Range.h"
#include "Tile.h"
#include "IConfigSection.h"
#include "OpenStreetMapDataProvider.h"
#include "Primitives/Range.h"
#include "AggregateMapDataProvider.generated.h"


namespace UtyMap { namespace Unreal { namespace Maps { namespace Providers { class FMapzenMapDataProvider; } } } }
namespace UtyMap { namespace Unreal { namespace Maps { namespace Providers { class FMapzenElevationDataProvider; } } } }
namespace UtyMap { namespace Unreal { namespace Maps { namespace Providers { class FSrtmElevationDataProvider; } } } }

using namespace UtyDepend;
using namespace UtyDepend::Config;


// Aggregates different map data providers and decides which one to use for specific tiles. 
UCLASS()
class UAggregateMapDataProvider : public UMapDataProvider, public IConfigurable
{
	GENERATED_BODY()

public:
	//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
	//ORIGINAL LINE: [Dependency] public FAggregateMapDataProvider(IFileSystemService fileSystemService, INetworkService networkService, ITrace trace)

	void Configure(IConfigSection* configSection);

private:
	TLodRange<int>* const  OsmTileRange = new TLodRange<int>(16, 16);

	UOpenStreetMapDataProvider*  _osmMapDataProvider;
	UtyMap::Unreal::Maps::Providers::FMapzenMapDataProvider* _mapzenMapDataProvider;

	UtyMap::Unreal::Maps::Providers::FMapzenElevationDataProvider* _mapzenElevationDataProvider;
	UtyMap::Unreal::Maps::Providers::FSrtmElevationDataProvider* _srtmElevationDataProvider;

	EElevationDataType _eleDataType;
};
