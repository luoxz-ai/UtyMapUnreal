#include "UtyMapUnreal.h"
#include "ConfigBuilder.h"
#include "ElevationDataType.h"

using namespace UtyDepend::Config;

namespace UtyMap
{
	namespace Unreal
	{
		namespace Infrastructure
		{
			namespace Config
			{
				void FConfigBuilder::Add(const std::string& path, std::string value)
				{
					_configSection->Add(path, value);
				}

				IConfigSection* FConfigBuilder::Build()
				{
					return _configSection;
				}

				FConfigBuilder* FConfigBuilder::SetSrtmEleData(const std::string& url, const std::string& schema)
				{
					Add(std::string("data/srtm/server"), url);
					Add(std::string("data/srtm/schema"), schema);
					return this;
				}

				FConfigBuilder* FConfigBuilder::SetGeocodingServer(const std::string& url)
				{
					Add(std::string("geocoding"), url);
					return this;
				}

				FConfigBuilder* FConfigBuilder::SetLocalElevationData(const std::string& path)
				{
					Add(std::string("data/elevation/local"), path);
					return this;
				}

				FConfigBuilder* FConfigBuilder::SetStringIndex(const std::string& path)
				{
					Add(std::string("data/index/strings"), path);
					return this;
				}

				FConfigBuilder* FConfigBuilder::SetSpatialIndex(const std::string& path)
				{
					Add(std::string("data/index/spatial"), path);
					return this;
				}

				FConfigBuilder* FConfigBuilder::SetOsmMapData(const std::string& url, const std::string& schema, const std::string& format)
				{
					Add(std::string("data/osm/server"), url);
					Add(std::string("data/osm/query"), schema);
					Add(std::string("data/osm/format"), format);
					return this;
				}

				FConfigBuilder* FConfigBuilder::SetMapzenMapData(const std::string& url, const std::string& layers, const std::string& format, const std::string& apiKey)
				{
					Add(std::string("data/mapzen/server"), url);
					Add(std::string("data/mapzen/layers"), layers);
					Add(std::string("data/mapzen/format"), format);
					Add(std::string("data/mapzen/apikey"), apiKey);
					return this;
				}

				FConfigBuilder* FConfigBuilder::SetMapzenEleData(const std::string& url, int gridSize, const std::string& format, const std::string& apiKey)
				{
					Add(std::string("data/mapzen/ele_server"), url);
					Add(std::string("data/mapzen/ele_grid"), std::to_string(gridSize));
					Add(std::string("data/mapzen/ele_format"), format);
					Add(std::string("data/mapzen/api_key"), apiKey);

					return this;
				}

				FConfigBuilder* FConfigBuilder::SetCache(const std::string& cache)
				{
					Add(std::string("data/cache"), cache);
					return this;
				}

				FConfigBuilder* FConfigBuilder::SetTileDistance(int distance)
				{
					Add(std::string(R"(tile/max_tile_distance)"), std::to_string(distance));
					return this;
				}

				FConfigBuilder* FConfigBuilder::SetElevationType(EElevationDataType type)
				{
					Add(std::string(R"(data/elevation/type)"), std::to_string(static_cast<int>(type)));
					return this;
				}

				FConfigBuilder* FConfigBuilder::GetDefault()
				{
					//return this;
					FConfigBuilder* tempVar = new FConfigBuilder();
					tempVar->SetLocalElevationData(std::string("Index/"))
					->SetSrtmEleData(std::string("http://dds.cr.usgs.gov/srtm/version2_1/SRTM3"),
						std::string("Config/srtm.schema.txt"))->SetOsmMapData(std::string("http://api.openstreetmap.org/api/0.6/map?bbox="),
							std::string("{1},{0},{3},{2}"),
							std::string("xml"))->SetMapzenMapData(std::string("http://tile.mapzen.com/mapzen/vector/v1/{0}/{1}/{2}/{3}.json?api_key={4}"),
								std::string("all"),
								std::string("json"),
								std::string(""))->SetMapzenEleData(std::string("http://elevation.mapzen.com/height?json={0}&api_key={1}"),
									4,
									std::string("ele"),
									std::string(""))->SetCache(std::string("Cache"))->SetGeocodingServer(std::string("http://nominatim.openstreetmap.org/search?"))->SetElevationType(EElevationDataType::Flat);
					//.SetOsmMapData("http://overpass-api.de/api/interpreter?data=", "(node({0},{1},{2},{3}); <; >;);out body;", "xml")
					return tempVar;
				}
			}
		}
	}
}
