#pragma once

#include "IMapDataProvider.h"
#include "IConfigurable.h"
#include "Tile.h"
#include "IConfigSection.h"
#include <string>
#include "OpenStreetMapDataProvider.generated.h"

using namespace UtyDepend;
using namespace UtyDepend::Config;

// Downloads map data from openstreemap servers.
UCLASS()
class UOpenStreetMapDataProvider : public UObject, public IConfigurable
{
	GENERATED_BODY()

private:
	std::string _cachePath;
	std::string _mapDataFormatExtension;
	std::string _mapDataServerQuery;
	std::string _mapDataServerUri;

public:
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [Dependency] public FOpenStreetMapDataProvider(IFileSystemService fileSystemService, INetworkService networkService, ITrace trace) : base(fileSystemService, networkService, trace)
	//FOpenStreetMapDataProvider(); // IFileSystemService* fileSystemService, INetworkService* networkService, ITrace* trace);

	
	virtual std::string* Get(ATile* tile);

	
	void Configure(IConfigSection* configSection);
};
