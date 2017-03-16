#include "UtyMapUnreal.h"
#include "NominatimGeocoder.h"

using namespace UtyDepend;
using namespace UtyDepend::Config;

namespace UtyMap
{
	namespace Unreal
	{
		namespace Maps
		{
			namespace Geocoding
			{

const std::string NominatimGeocoder::DefaultSearchServer = std::string(R"(http://nominatim.openstreetmap.org/search?)");
const std::string NominatimGeocoder::DefaultReverseSearchServer = std::string(R"(http://nominatim.openstreetmap.org/reverse?)");

NominatimGeocoder::NominatimGeocoder() // INetworkService* networkService) : _networkService(networkService)
				{
				}

				FGeocoderResult* NominatimGeocoder::Search(const std::string& name)
				{
					return Search(name, nullptr);
				}

				FGeocoderResult* NominatimGeocoder::Search(const std::string& name, FBoundingBox* area)
				{
					return nullptr;
					/*auto sb = new StringBuilder(128);
					sb->append(_searchPath);
					if (area != nullptr)
					{
						sb->append(Uri::EscapeDataString(std::string::Format(CultureInfo::GetInvariantCulture(), std::string("viewbox={0:f4},{1:f4},{2:f4},{3:f4}&"), area->MinPoint.Longitude, area->MinPoint.Latitude, area->MaxPoint.Longitude, area->MaxPoint.Latitude)));
					}
					sb->AppendFormat(std::string("q={0}&format=json"), Uri::EscapeDataString(name));

					return _networkService->Get(sb->toString(), _headers)->Take(1)->SelectMany([ & ] (void* r)
					{
											(from FJSONNode json in JSON::Parse(r)->AsArray select ParseGeocoderResult(json));
					});*/
				}

				FGeocoderResult* NominatimGeocoder::Search(FGeoCoordinate* coordinate)
				{
					return nullptr;
					/*auto url = std::string::Format(std::string("{0}format=json&lat={1}&lon={2}"), _reverseSearchPath, coordinate->Latitude, coordinate->Longitude);

					return _networkService->Get(url, _headers)->Take(1)->Select([ & ] (void* r)
					{
											ParseGeocoderResult(JSON::Parse(r));
					});*/
				}
//
//				FGeocoderResult* NominatimGeocoder::ParseGeocoderResult(FJSONNode* resultNode)
//				{
//					FBoundingBox* bbox = nullptr;
////C# TO C++ CONVERTER WARNING: C# to C++ Converter has converted this array to a pointer. You will need to call 'delete[]' where appropriate:
////ORIGINAL LINE: string[] bboxArray = resultNode["boundingbox"].Value.Split(',');
////C# TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to this .NET String method:
//					std::string* bboxArray = resultNode[ std::string("boundingbox") ]->Value->Split(',');
////C# TO C++ CONVERTER TODO TASK: Native C++ has no direct way to determine the length or upper bound of a specific dimension of an array:
//					if (bboxArray->GetLength() == 4)
//					{
//						bbox = new FBoundingBox(ParseGeoCoordinate(bboxArray[ 0 ], bboxArray[ 2 ]), ParseGeoCoordinate(bboxArray[ 1 ], bboxArray[ 3 ]));
//					}
//
//					FGeocoderResult* tempVar = new FGeocoderResult();
//					tempVar->PlaceId = std::stoll(resultNode[ std::string("place_id") ]->Value);
//					tempVar->OsmId = std::stoll(resultNode[ std::string("osm_id") ]->Value);
//					tempVar->OsmType = resultNode[ std::string("osm_type") ]->Value;
//					tempVar->DisplayName = resultNode[ std::string("display_name") ]->Value;
//					tempVar->Class = resultNode[ std::string("class") ]->Value;
//					tempVar->Type = resultNode[ std::string("type") ]->Value;
//					tempVar->Coordinate = ParseGeoCoordinate(resultNode[ std::string("lat") ]->Value, resultNode[ std::string("lon") ]->Value);
//					tempVar->BoundginBox = bbox;
//					return tempVar;
//				}
//
				FGeoCoordinate* NominatimGeocoder::ParseGeoCoordinate(const std::string& latStr, const std::string& lonStr)
				{
					double latitude = std::stod(latStr);
					double longitude = std::stod(lonStr);
					if( (latitude != NAN) && (longitude != NAN))
					{
						return new FGeoCoordinate(latitude, longitude);
					}
					return new FGeoCoordinate;
				}

				void NominatimGeocoder::Configure(IConfigSection* configSection)
				{
					_searchPath = configSection->GetString(std::string("geocoding"), DefaultSearchServer);
					_reverseSearchPath = configSection->GetString(std::string("reverse_geocoding"), DefaultReverseSearchServer);
				}
			}
		}
	}
}
