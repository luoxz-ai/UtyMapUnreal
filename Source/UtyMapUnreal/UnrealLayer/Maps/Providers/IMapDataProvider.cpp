#include "UtyMapUnreal.h"
#include "IMapDataProvider.h"

std::string* UMapDataProvider::Get(ATile* tile, const std::string& uri, const std::string& filePath)
{
	return new std::string;

//	if (_fileSystemService->Exists(filePath))
//	{
//		return Observable::Return<std::string>(filePath);
//	}
//
//	return Observable::Create<std::string>([ & ] (void* observer)
//	{
//							_trace->Warn(TraceCategory, FStaticStrings::NoPresistentElementSourceFound, tile->QuadKey.ToString(), uri);
//							_networkService->GetAndGetBytes(uri).ObserveOn(Scheduler->ThreadPool).Subscribe([ & ] (void* bytes)
//							{
//					_trace->Debug(TraceCategory, std::string("saving bytes: {0}"), std::to_string(bytes->Length));
//					{
//									std::lock_guard<std::mutex> lock(_lockObj);
//						if (!_fileSystemService->Exists(filePath))
//						{
////C# TO C++ CONVERTER NOTE: The following 'using' block is replaced by its C++ equivalent:
////ORIGINAL LINE: return Observable.Create<string>(observer =>
//							{
//								auto stream = _fileSystemService->WriteStream(filePath);
//								WriteBytes(stream, bytes);
//							}
//						}
//					}
//					observer::OnNext(filePath);
//					observer::OnCompleted();
//							});
//                            
//							return Disposable::Empty;
//	});
}

void UMapDataProvider::WriteBytes(std::ifstream* stream, unsigned char bytes[])
{
////C# TO C++ CONVERTER TODO TASK: Native C++ has no direct way to determine the length or upper bound of a specific dimension of an array:
//					stream->Write(bytes, 0, bytes->GetLength());
}
		