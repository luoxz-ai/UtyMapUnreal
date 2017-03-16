#include "UtyMapUnreal.h"
#include "SrtmElevationDataProvider.h"

using namespace UtyDepend::Config;

namespace UtyMap
{
	namespace Unreal
	{
		namespace Maps
		{
			namespace Providers
			{

const std::unordered_map<int, std::string> FSrtmElevationDataProvider::ContinentMap = std::unordered_map<int, std::string>
{
	{0, std::string("Eurasia")},
	{1, std::string("South_America")},
	{2, std::string("Africa")},
	{3, std::string("North_America")},
	{4, std::string("Australia")},
	{5, std::string("Islands")}
};

FSrtmElevationDataProvider::FSrtmElevationDataProvider() //IFileSystemService* fileSystemService, INetworkService* networkService, ITrace* trace) : _fileSystemService(fileSystemService), _networkService(networkService), _trace(trace)
				{
				}

				std::string* FSrtmElevationDataProvider::Get(ATile* tile)
				{
//					// TODO tile can cross more than one srtm cell: need load more.
//					auto prefix = GetFileNamePrefix(tile->BoundingBox.Center());
//					auto filePath = FileSystem::combine(_elePath, prefix);
//
//					if (_fileSystemService->Exists(filePath))
//					{
//						return Observable::Return(filePath);
//					}
//
//					return Download(prefix)->Select([ & ] (void* bytes)
//					{
//											{
//															std::lock_guard<std::mutex> lock(_lock);
//							if (!_fileSystemService->Exists(filePath))
//							{
//								_trace->Info(TraceCategory, std::string("Unzipping {0} bytes"), std::to_string(bytes->Length));
//								auto hgtData = FCompressionUtils::Unzip(bytes).Single()->Value;
////C# TO C++ CONVERTER NOTE: The following 'using' block is replaced by its C++ equivalent:
////ORIGINAL LINE: return Download(prefix).Select(bytes =>
//								{
//									auto stream = _fileSystemService->WriteStream(filePath);
//									stream->Write(hgtData, 0, hgtData->Length);
//								}
//							}
//											}
//                            
//											return filePath;
//					});
					return nullptr;
				}

				void FSrtmElevationDataProvider::Configure(IConfigSection* configSection)
				{
					_server = configSection->GetString(std::string(R"(data/srtm/server)"));
					_schemaPath = configSection->GetString(std::string(R"(data/srtm/schema)"));
					_elePath = configSection->GetString(std::string(R"(data/elevation/local)"));
				}

				std::string FSrtmElevationDataProvider::GetFileNamePrefix(FGeoCoordinate* coordinate)
				{
					/*return std::string::Format(std::string("{0}{1:00}{2}{3:000}"), coordinate->Latitude > 0 ? 'N' : 'S', std::abs(static_cast<int>(coordinate->Latitude)), coordinate->Longitude > 0 ? 'E' : 'W', std::abs(static_cast<int>(coordinate->Longitude)));*/
					return std::string("");
				}
//
//				IObservable<unsigned char[]>* FSrtmElevationDataProvider::Download(const std::string& prefix)
//				{
//					// TODO do not read schema file for every srtm cell.
////C# TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to this .NET String method:
//					for (unknown::const_iterator line = _fileSystemService->ReadText(_schemaPath)->Split('\n').begin(); line != _fileSystemService->ReadText(_schemaPath)->Split('\n').end(); ++line)
//					{
////C# TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to this .NET String method:
//						if ((*line)->StartsWith(prefix))
//						{
////C# TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to this .NET String method:
//							auto parameters = (*line)->Split(' ');
//							// NOTE some of files miss extension point between name and .hgt.zip
////C# TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to this .NET String method:
//							auto url = std::string::Format(std::string("{0}/{1}/{2}"), _server, ContinentMap[ std::stoi(parameters[ 1 ]) ], parameters[ 1 ]->EndsWith(std::string("zip")) ? std::string("") : parameters[ 0 ] + std::string(std::string(".hgt.zip")));
//							_trace->Warn(TraceCategory, std::string::Format(std::string("Downloading SRTM data from {0}"), url));
//							return _networkService->GetAndGetBytes(url);
//						}
//					}
//					return Observable::Throw<unsigned char[]>(std::invalid_argument(std::string::Format(std::string("Cannot find {0} on {1}"), prefix, _schemaPath)));
//				}
			}
		}
	}
}
