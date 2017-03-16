#pragma once

#include "ElevationDataType.h"
#include "CodeConfigSection.h"
#include <string>


namespace UtyMap { namespace Unreal { namespace Infrastructure { namespace Config { class FCodeConfigSection; } } } }

using namespace UtyDepend::Config;

namespace UtyMap{
	namespace Unreal
	{
		namespace Infrastructure
		{
			namespace Config
			{
				// Represents a builder responsible for application configuration creation.
				class FConfigBuilder
				{
				private:
					FCodeConfigSection* const  _configSection = new FCodeConfigSection();

					// Adds value to configuration with path provided.
					/// <remarks> Use this method to extend application with your custom settings. </remarks>
					/// <typeparam name="T">Type of value.</typeparam>
					// @param path">Path
					// @param value">Value
				public:
					void Add(const std::string& path, std::string value);

					// Builds application specific configuration.
					IConfigSection* Build();


					// Sets settings to get elevation data from remote server.
					FConfigBuilder* SetSrtmEleData(const std::string& url, const std::string& schema);

					// Sets geocoding server's url.
					FConfigBuilder* SetGeocodingServer(const std::string& url);

					// Sets local path to elevation data.
					FConfigBuilder* SetLocalElevationData(const std::string& path);

					// Sets string index.
					FConfigBuilder* SetStringIndex(const std::string& path);

					// Sets string index.
					FConfigBuilder* SetSpatialIndex(const std::string& path);

					// Sets osm map data provider.
					FConfigBuilder* SetOsmMapData(const std::string& url, const std::string& schema, const std::string& format);

					// Sets mapzen map data provider.
					FConfigBuilder* SetMapzenMapData(const std::string& url, const std::string& layers, const std::string& format, const std::string& apiKey);

					FConfigBuilder* SetMapzenEleData(const std::string& url, int gridSize, const std::string& format, const std::string& apiKey);

					// Sets cache data path.
					FConfigBuilder* SetCache(const std::string& cache);

					// Sets distance (in tile count) between current and tile to be unloaded.
					FConfigBuilder* SetTileDistance(int distance);

					FConfigBuilder* SetElevationType(EElevationDataType type);
					
					// Gets ConfigBuilder with default settings.
					/// <remarks> You can call methods to override settings with custom ones. </remarks>
					static FConfigBuilder* GetDefault();

				};
			}
		}
	}
}
