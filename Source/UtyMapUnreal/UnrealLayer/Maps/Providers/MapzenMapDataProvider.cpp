#include "UtyMapUnreal.h"
#include "MapzenMapDataProvider.h"

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

				FMapzenMapDataProvider::FMapzenMapDataProvider()
				{
				}

				std::string* FMapzenMapDataProvider::Get(ATile* tile)
				{
					//auto filePath = FileSystem::combine(_cachePath, tile->QuadKey + _mapDataFormatExtension);
					//auto uri = std::string::Format(_mapDataServerUri, _mapDataLayers, tile->QuadKey.LevelOfDetail, tile->QuadKey.TileX, tile->QuadKey.TileY, _mapDataApiKey);

					//return Get(tile, uri, filePath);
					return nullptr;
				}


				void FMapzenMapDataProvider::Configure(IConfigSection* configSection)
				{
					_mapDataServerUri = configSection->GetString(std::string(R"(data/mapzen/server)"), nullptr);
					_mapDataFormatExtension = std::string(std::string(".")) + configSection->GetString(std::string(R"(data/mapzen/format)"), std::string("json"));
					_mapDataApiKey = configSection->GetString(std::string(R"(data/mapzen/apikey)"), nullptr);
					_mapDataLayers = configSection->GetString(std::string(R"(data/mapzen/layers)"), nullptr);

					_cachePath = configSection->GetString(std::string(R"(data/cache)"), nullptr);
				}
			}
		}
	}
}
