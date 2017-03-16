#pragma once

#include "NoExportTypes.h"
#include "MapStorageType.h"
#include "ElevationDataType.h"
#include "IConfigSection.h"
#include "IConfigurable.h"
#include "OpenStreetMapDataProvider.h"
#include "Stylesheet.h"
#include "Range.h"
#include "Tile.h"
#include "Mesh.h"
#include "Element.h"
#include <string>
#include <cstring>
#include <mutex>
#include "Runtime/Core/Public/Containers/Union.h"
#include "MapTileAdapter.h"
#include "ConfigBuilder.h"
#include "Range.h"
#include "MapdataLoader.generated.h"

// TODO: make MapDataLoader async

using namespace UtyDepend;
using namespace UtyDepend::Config;

UCLASS()
class UMapDataLoader : public UObject
{
	GENERATED_BODY()

private:
	std::mutex _lockGuard;

	UOpenStreetMapDataProvider*  _mapDataProvider;
	EElevationDataType _eleDataType = EElevationDataType::Flat;
	UPROPERTY()
	ADemoModelBuilder* _modelBuilder;
	UPROPERTY()
	UMapTileAdapter* _mapTileAdapter;

public:
	UMapDataLoader(UOpenStreetMapDataProvider* mapDataProvider);
	UMapDataLoader();
	~UMapDataLoader();


	UFUNCTION(BlueprintCallable, Category = "MyProject")
	void Load(ATile* tile);

	void Configure(IConfigSection* configSection);
	void Init(ADemoModelBuilder* ModelBuilder, UOpenStreetMapDataProvider* mapDataProvider, std::string stringPath, std::string mapDataPath, std::string MapCssFile, EElevationDataType eleDataType, TLodRange<int32> LevelOfDetail);
	void AddToStore(EMapStorageType storageType, const std::string& dataPath, UStylesheet* stylesheet, TLodRange<int>* levelOfDetails);
	void Shutdown();
	void FetchUpdates();
	int FetchAndResetOutstandingArrivedElements();

private:
	void CreateLoadSequence(ATile* tile);
	void SaveTileData(ATile* tile, const std::string& filePath);
};