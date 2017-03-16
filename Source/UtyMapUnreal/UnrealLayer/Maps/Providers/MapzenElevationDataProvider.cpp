#include "UtyMapUnreal.h"
#include "MapzenElevationDataProvider.h"

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

				FMapzenElevationDataProvider::FMapzenElevationDataProvider() // IFileSystemService* fileSystemService, INetworkService* networkService, ITrace* trace) : FMapDataProvider(fileSystemService, networkService, trace)
				{
				}

				std::string* FMapzenElevationDataProvider::Get(ATile* tile)
				{
//C# TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'ToString':
					//auto directory = FileSystem::combine(_elePath, tile->QuadKey.LevelOfDetail.ToString());
					//auto filePath = FileSystem::combine(directory, tile->QuadKey + _mapDataFormatExtension);
					//auto uri = std::string::Format(_mapDataServerUri, GetJsonPayload(tile->QuadKey), _mapDataApiKey);

					//return Get(tile, uri, filePath);
					return nullptr;
				}

				void FMapzenElevationDataProvider::WriteBytes(void* stream, unsigned char bytes[])
				{
//					auto content = Encoding::GetUTF8()->GetString(bytes);
//					auto json = JSON::Parse(content);
//					auto heights = json[ std::string("height") ].AsArray;
//
//					StringBuilder* sb = new StringBuilder(1024);
//					for (var::const_iterator jsonHeight = heights->begin(); jsonHeight != heights->end(); ++jsonHeight)
//					{
//						sb->AppendFormat(std::string("{0} "), (*jsonHeight)->Value);
//					}
//					sb->remove(sb->GetLength() - 1, 1);
//
//					auto intBytes = Encoding::GetUTF8()->GetBytes(sb->toString());
//
////C# TO C++ CONVERTER TODO TASK: Native C++ has no direct way to determine the length or upper bound of a specific dimension of an array:
//					stream->Write(intBytes, 0, intBytes->GetLength());
				}

				std::string FMapzenElevationDataProvider::GetJsonPayload(FQuadKey* quadkey)
				{
					//return std::string::Format(std::string("{{\"range\":false,\"encoded_polyline\":\"{0}\"}}"), EncodePolyline(CreatePolyline(quadkey)));
					return std::string("");
				}

				std::vector<FGeoCoordinate*> FMapzenElevationDataProvider::CreatePolyline(FQuadKey* quadkey)
				{
//					auto bbox = FGeoUtils::QuadKeyToBoundingBox(quadkey);
//
//					auto latStep = (bbox->MaxPoint.Latitude - bbox->MinPoint.Latitude) / _eleGrid;
//					auto lonStep = (bbox->MaxPoint.Longitude - bbox->MinPoint.Longitude) / _eleGrid;
//
//					for (int i = 0; i <= _eleGrid; ++i)
//					{
//						auto lat = bbox->MinPoint.Latitude + i * latStep;
//						for (int j = 0; j <= _eleGrid; ++j)
//						{
//							auto lon = bbox->MinPoint.Longitude + j * lonStep;
////C# TO C++ CONVERTER TODO TASK: C++ does not have an equivalent to the C# 'yield' keyword:
//							yield return new FGeoCoordinate(lat, lon);
//						}
//					}
					return std::vector<FGeoCoordinate*>();
				}

				std::string FMapzenElevationDataProvider::EncodePolyline(std::vector<FGeoCoordinate*>& points)
				{
					constexpr double precision = 1E6;
/*
					auto str = new StringBuilder();
					std::function<void(int)> encodeDiff = ([ & ] (void* diff)
					{
											int shifted = diff << 1;
											if (diff < 0)
											{
							shifted = ~shifted;
											}
											int rem = shifted;
											while (rem >= 0x20)
											{
							str->append(static_cast<char>((0x20 | (rem & 0x1f)) + 63));
							rem >>= 5;
											}
											str->append(static_cast<char>(rem + 63));
					});

					int lastLat = 0;
					int lastLng = 0;
					for (std::vector<FGeoCoordinate*>::const_iterator point = points.begin(); point != points.end(); ++point)
					{
						int lat = static_cast<int>(std::round((*point)->Latitude*precision));
						int lng = static_cast<int>(std::round((*point)->Longitude*precision));
						encodeDiff(lat - lastLat);
						encodeDiff(lng - lastLng);
						lastLat = lat;
						lastLng = lng;
					}
					return str->toString();*/
					return nullptr;
				}

				void FMapzenElevationDataProvider::Configure(IConfigSection* configSection)
				{
					_mapDataServerUri = configSection->GetString(std::string(R"(data/mapzen/ele_server)"));
					_mapDataApiKey = configSection->GetString(std::string(R"(data/mapzen/api_key)"));
					_mapDataFormatExtension = std::string(std::string(".")) + configSection->GetString(std::string(R"(data/mapzen/ele_format)"), std::string("ele"));

					// TODO this parameter depends on height data precision and tile size
					_eleGrid = configSection->GetInt(std::string(R"(data/mapzen/ele_grid)"), 10);

					_elePath = configSection->GetString(std::string(R"(data/elevation/local)"), nullptr);
				}
			}
		}
	}
}
