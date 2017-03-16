#include "UtyMapUnreal.h"
#include "MapDataLoader.h"
#include "MapTileAdapter.h"
#include "CoreLibrary.h"
#include "DemoModelBuilder.h"

using namespace UtyDepend;
using namespace UtyDepend::Config;

UMapDataLoader::UMapDataLoader(UOpenStreetMapDataProvider* mapDataProvider/*, FMapTileAdapter* tileAdapter*/) : _mapDataProvider(mapDataProvider) //, _tileAdapter(tileAdapter)
{

}

UMapDataLoader::UMapDataLoader() : _mapDataProvider(nullptr)
{

}

void UMapDataLoader::AddToStore(EMapStorageType storageType, const std::string& dataPath, UStylesheet* stylesheet, TLodRange<int>* levelOfDetails)
{
	auto dataPathResolved = dataPath; // todo: implement _pathResolver->Resolve(dataPath);
	auto stylesheetPathResolved = stylesheet->GetPath();  // todo: implement ->Resolve(stylesheet->Path);
	auto tempStorageKey = storageType;
					
	std::string errorMsg = std::string("");
	UCoreLibrary::AddToStore(&storageType, stylesheetPathResolved, dataPathResolved, levelOfDetails);

	if (errorMsg != std::string(""))
	{
		UE_LOG(UtyProcessing, Error, TEXT("%s"), UTF8_TO_TCHAR(errorMsg.c_str()));
	}
}

void UMapDataLoader::Load(ATile* tile)
{
	CreateLoadSequence(tile);

	/*auto newTile = _mapDataProvider->Get(tile);
	return _mapDataProvider->Get(tile)->SelectMany([ & ] (const std::string& filePath)
	{
				if (!UCoreLibrary::HasData(tile->QuadKey))
				{
					SaveTileData(tile, filePath);
				}
                       
				return CreateLoadSequence(tile);
	});*/
}

void UMapDataLoader::Configure(IConfigSection* configSection)
{
	//auto stringPath = /*_pathResolver->Resolve(*/ configSection->GetString(std::string("data/index/strings"));
	//auto mapDataPath = /*_pathResolver->Resolve(*/ configSection->GetString(std::string("data/index/spatial"));
	//_eleDataType = static_cast<EElevationDataType>(configSection->GetInt(std::string("data/elevation/type"), 0));

	UE_LOG(UtyProcessing, Error, TEXT("Use Init function instead"));
	//if (_mapTileAdapter == nullptr)
	//{
	//	_mapTileAdapter = NewObject<UMapTileAdapter>(this, TEXT("MapTileAdapter"), RF_Transactional);
	//	_mapTileAdapter->AddToRoot();
	//	UE_LOG(UtyProcessing, Warning, TEXT("Created  a new MapTileAdapter"));
	//	return;
	//}

	//UCoreLibrary::Configure(stringPath, mapDataPath, Levelofd_mapTileAdapter);
}


void UMapDataLoader::Init(ADemoModelBuilder* ModelBuilder, UOpenStreetMapDataProvider* mapDataProvider, std::string IndexFolderPath, std::string mapDataPath, std::string MapCssFile, EElevationDataType eleDataType, TLodRange<int32> LevelOfDetail)
{
	_mapDataProvider = mapDataProvider;
	_eleDataType = eleDataType;
	if (_modelBuilder != nullptr)
	{
		if (_modelBuilder->IsValidLowLevel())
			UE_LOG(UtyProcessing, Display, TEXT("Deleting old modelbuilder"));
			_modelBuilder->MarkPendingKill();
	}
	_modelBuilder = ModelBuilder;
	//auto stringPath = /*_pathResolver->Resolve(*/ configSection->GetString(std::string("data/index/strings"));
	//auto mapDataPath = /*_pathResolver->Resolve(*/ configSection->GetString(std::string("data/index/spatial"));
	//_eleDataType = static_cast<EElevationDataType>(configSection->GetInt(std::string("data/elevation/type"), 0));

	UE_LOG(UtyProcessing, Display, TEXT("Configuring UtyMap core lbirary with Map folder: %s and MapCSS style folder: %s and ElevationType: todo"), UTF8_TO_TCHAR(mapDataPath.c_str()), UTF8_TO_TCHAR(IndexFolderPath.c_str()));
	// adapter is only needed for OnError callback. would need to call Init if more setup is needed
	if (_mapTileAdapter == nullptr)
	{
		_mapTileAdapter = NewObject<UMapTileAdapter>(this, TEXT("MapTileAdapter"), RF_Transactional);
		UE_LOG(UtyProcessing, Warning, TEXT("Created a new MapTileAdapter"));
	}

	UCoreLibrary::Configure(IndexFolderPath, mapDataPath, MapCssFile, LevelOfDetail, _mapTileAdapter);
}

void UMapDataLoader::Shutdown()
{
	_mapTileAdapter->Shutdown();
}

int UMapDataLoader::FetchAndResetOutstandingArrivedElements()
{
	if (_mapTileAdapter)
	{
		if (_mapTileAdapter->IsValidLowLevel())
		{
			return _mapTileAdapter->BroadCastNew();
		}
	}
	return 0;
}

UMapDataLoader::~UMapDataLoader()
{
	//delete UCoreLibrary;
}

void UMapDataLoader::CreateLoadSequence(ATile* tile)
{
	auto stylesheetPathResolved = tile->GetStylesheet()->GetPath();						
	
	//UE_LOG(UtyProcessing, Display, TEXT("loading tile: %s using style: %s"), UTF8_TO_TCHAR(tile->ToString().c_str()), UTF8_TO_TCHAR(stylesheetPathResolved.c_str()));
	
	if (_mapTileAdapter == nullptr)
	{
		UE_LOG(UtyProcessing, Error, TEXT("No valid MapTileAdapter available"));
		return;
	}
	_mapTileAdapter->Init(tile, _modelBuilder);
	//adapter->SubscribeFinishedEvent(&UMapDataLoader::OnLoaded);
	
	UCoreLibrary::LoadQuadKey(stylesheetPathResolved, tile->GetQuadKey(), &_eleDataType);
							
	//UE_LOG(UtyProcessing, Display, TEXT("tile loaded: %s"), UTF8_TO_TCHAR(tile->ToString().c_str()));
	//observer::OnCompleted();
}

void UMapDataLoader::SaveTileData(ATile* tile, const std::string& filePath)
{
	{
		std::lock_guard<std::mutex> lock(_lockGuard);
		if (UCoreLibrary::HasData(tile->GetQuadKey()))
		{
			return;
		}

		auto filePathResolved = /*_pathResolver->Resolve(*/filePath;
		auto stylesheetPathResolved = /*_pathResolver->Resolve(*/tile->GetStylesheet()->GetPath();

		//Trace->Info(TraceCategory, std::string::Format(std::string("save tile data {0} from {1} using style: {2}"), tile, filePathResolved, stylesheetPathResolved));

		std::string errorMsg = std::string("");
		//UCoreLibrary::AddToStore(EMapStorageType::Persistent, stylesheetPathResolved, filePathResolved, tile->QuadKey, [ & ] (const std::string& error)
		//{
		//	errorMsg = error;
		//});

		if (errorMsg != std::string(""))
		{
			// todo: log
			//throw FMapDataException(std::string::Format(FStaticStrings::CannotAddDataToInMemoryStore, errorMsg));
		}
	}
}
