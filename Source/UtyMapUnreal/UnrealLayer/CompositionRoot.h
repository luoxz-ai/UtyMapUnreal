#pragma once

#include "UtyMapUnreal.h"
#include <vector>
#include <algorithm>
#include <functional>
#include "Element.h"
#include "Tile.h"
#include "ConfigBuilder.h"
#include "MapDataLoader.h"
#include "NominatimGeocoder.h"
#include "Strings.h"
#include "ModelBuilder.h"
#include "ElementEditor.h"
#include "TileController.h"
#include "AggregateMapDataProvider.h"
#include "MapElevationLoader.h"
#include "Stylesheet.h"
#include "OpenStreetMapDataProvider.h"
#include "CompositionRoot.generated.h"


using namespace UtyMap::Unreal::Infrastructure;
using namespace UtyMap::Unreal::Maps::Data;
using namespace UtyMap::Unreal::Maps::Geocoding;


/**
* Represents application's composition root.
*
*  At this level, application setups object graph by specifying concrete implementations.
*
* @see https://docs.unrealengine.com/latest/INT/Gameplay/Framework/Controller/
*/
UCLASS(BlueprintType, Blueprintable)
class ACompositionRoot : public AActor
{
	GENERATED_UCLASS_BODY()

private:
	bool _isInitialized = false;
	//IContainer* const  _container;
	//IConfigSection* const  _configSection;
	//const std::vector<std::function<void(IContainer*, IConfigSection*)>*> _bootstrapperActions;

	UStylesheet* Stylesheet;
	FModelBuilder* ModelBuilder;
	FElementEditor* ElementEditor;
	ATileController* TileController;
	UOpenStreetMapDataProvider* AggregateDataProvider;
	UMapDataLoader* MapDataLoader;
	FMapElevationLoader* MapElevationLoader;

public:

	/**
	* Performs object graph setup.
	*
	* @return Returns if setup was successful
	*/
	UFUNCTION(BlueprintCallable, Category = "UtyMap")
	bool Setup();
};