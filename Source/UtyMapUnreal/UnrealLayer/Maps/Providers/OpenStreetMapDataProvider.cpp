#include "UtyMapUnreal.h"
#include "OpenStreetMapDataProvider.h"

using namespace UtyDepend;
using namespace UtyDepend::Config;

std::string* UOpenStreetMapDataProvider::Get(ATile* tile)
{
	////auto filePath = FileSystem::combine(_cachePath, tile->QuadKey + _mapDataFormatExtension);
	//auto padding = 0.001;
	//auto query = tile->GetBoundingBox();
	//auto queryString = std::string::Format(_mapDataServerQuery, query->MinPoint.Latitude - padding, query->MinPoint.Longitude - padding, query->MaxPoint.Latitude + padding, query->MaxPoint.Longitude + padding);
	//auto uri = std::string::Format(std::string("{0}{1}"), _mapDataServerUri, Uri::EscapeDataString(queryString));

	//return Get(tile, uri, filePath);
	return nullptr;
}

void UOpenStreetMapDataProvider::Configure(IConfigSection* configSection)
{
	_mapDataServerUri = configSection->GetString(std::string(R"(data/osm/server)"), nullptr);
	_mapDataServerQuery = configSection->GetString(std::string(R"(data/osm/query)"), nullptr);
	_mapDataFormatExtension = std::string(std::string(".")) + configSection->GetString(std::string(R"(data/osm/format)"), std::string("xml"));
	_cachePath = configSection->GetString(std::string(R"(data/cache)"), nullptr);
}
		