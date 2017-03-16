#include "UtyMapUnreal.h"
#include "AggregateMapDataProvider.h"
#include "OpenStreetMapDataProvider.h"
#include "MapzenMapDataProvider.h"
#include "MapzenElevationDataProvider.h"
#include "SrtmElevationDataProvider.h"

using namespace UtyDepend;
using namespace UtyDepend::Config;

//std::string* FAggregateMapDataProvider::Get(FTile* tile)
//{
//	/*return CreateEleDataObservable(tile)->SelectMany([ & ] (void* t)
//	{
//							CreateMapDataObservable(tile);
//	});*/
//	return nullptr;
//}

void UAggregateMapDataProvider::Configure(IConfigSection* configSection)
{
	_osmMapDataProvider->Configure(configSection);
	_mapzenMapDataProvider->Configure(configSection);

	_srtmElevationDataProvider->Configure(configSection);
	_mapzenElevationDataProvider->Configure(configSection);

	_eleDataType = static_cast<EElevationDataType>(configSection->GetInt(std::string("data/elevation/type"), 2));
}

//std::string* FAggregateMapDataProvider::CreateEleDataObservable(FTile* tile)
//{
//	//switch (_eleDataType)
//	//{
//	//	case EElevationDataType::Grid:
//	//		return _mapzenElevationDataProvider->Get(tile);
//	//	case EElevationDataType::Srtm:
//	//		return _srtmElevationDataProvider->Get(tile);
//	//}
//	//return Observable::Return(std::string(""));
//	return nullptr;
//}

//std::string* FAggregateMapDataProvider::CreateMapDataObservable(FTile* tile)
//{
//	if (UCoreLibrary::HasData(tile->GetQuadKey()))
//	{
//		//return Observable::Return(std::string(""));
//		return nullptr;
//	}

//	if (OsmTileRange->Contains(tile->GetQuadKey().LevelOfDetail))
//	{
//		return _osmMapDataProvider->Get(tile);
//	}

//	return _mapzenMapDataProvider->Get(tile);
//}
