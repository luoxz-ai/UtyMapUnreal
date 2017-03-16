#include "UtyMapUnreal.h"
#include "CompositionRoot.h"
#include "MapDataLoader.h"

using namespace UtyMap::Unreal::Infrastructure;
using namespace UtyMap::Unreal::Maps::Data;
using namespace UtyMap::Unreal::Maps::Geocoding;
using namespace UtyDepend;
using namespace UtyDepend::Config;



ACompositionRoot::ACompositionRoot(const FObjectInitializer& ObjectInitializer)
{

}

bool ACompositionRoot::Setup()
{
	if (_isInitialized)
	{
		UE_LOG(UtyInit, Error, TEXT("%s"), UTF8_TO_TCHAR(FStaticStrings::SetupIsCalledMoreThanOnce.c_str()));
		return false;
	}
/*
	ModelBuilder = NewObject<UModelBuilder>(this, TEXT("ModelBuilderModule"), RF_Transactional);
	ElementEditor = NewObject<FElementEditor>(this, TEXT("ElementBuilderModule"), RF_Transactional);
	TileController = NewObject<UTileController>(this, TEXT("TileControllerModule"), RF_Transactional);
	AggregateDataProvider = NewObject<FAggregateMapDataProvider>(this, TEXT("AggregateMapDataProviderModule"), RF_Transactional);
	MapDataLoader = NewObject<FMapDataLoader>(this, TEXT("MapDataLoaderModule"), RF_Transactional);
	MapElevationLoader = NewObject<FMapElevationLoader>(this, TEXT("MapElevationLoaderModule"), RF_Transactional);
*/
	Stylesheet = new UStylesheet(FStaticStrings::SetupIsCalledMoreThanOnce);
	ModelBuilder = new FModelBuilder; 
	ElementEditor = new FElementEditor(nullptr);
	//TileController = new ATileController;
	AggregateDataProvider = new UOpenStreetMapDataProvider;
	//MapDataLoader = new UMapDataLoader(AggregateDataProvider);
	MapElevationLoader = new FMapElevationLoader;

	//// TODO: get config file mechanism from unreal
	//_container->Register(FComponent->For<ITileController*>().Use<*>().SetConfig(_configSection));

	//_container->Register(FComponent->For<UMapDataProvider*>().Use<*>().SetConfig(_configSection));
	//_container->Register(FComponent->For<IMapDataLoader*>().Use<*>().SetConfig(_configSection));
	//_container->Register(FComponent->For<FMapElevationLoader*>().Use<*>().SetConfig(_configSection));

	//_container->Register(FComponent->For<IGeocoder*>().Use<NominatimGeocoder*>().SetConfig(_configSection));

	//// go through all actions to add/override services.
	//std::for_each(_bootstrapperActions.begin(), _bootstrapperActions.end(), [ & ] (void* action)
	//{
	//	action(_container, _configSection);
	//});

	_isInitialized = true;

	return true;
}

template<typename T>
T ACompositionRoot::GetService()
{
	return _container->Resolve<T>();
}

