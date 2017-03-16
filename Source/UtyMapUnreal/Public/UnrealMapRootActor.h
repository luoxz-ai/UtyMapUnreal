#pragma once

#include "GameFramework/Actor.h"
#include "TileController.h"
#include "MapDataLoader.h"
#include "TileController.h"
#include "DemoModelBuilder.h"
#include "IProjection.h"
#include "UnrealLayer/Core/GeoCoordinate.h"
#include "OpenStreetMapDataProvider.h"
#include "UnrealMapRootActor.generated.h"

// send new position into UtyMap processing
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPositionUpdateDelegate, FVector, Position, int32, LevelOfDetail);

UCLASS()
class UTYMAPUNREAL_API AUnrealMapRootActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnrealMapRootActor();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(BlueprintAssignable, Category = "MyProject")
		FPositionUpdateDelegate UpdatedPosition;

	UPROPERTY(GlobalConfig, EditAnywhere, Category = Settings, meta = (FilePathFilter = ".osm|.xml|.json"))
	FFilePath MapFile;
	UPROPERTY(GlobalConfig, EditAnywhere, Category = Settings, meta = (FilePathFilter = ".mapcss"))
	FFilePath MapCss;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtyMap Configuration")
	FString MapDataFileAsString = "C:/workspace/Projects/UtymapUnreal/utymap/unity/demo/Assets/StreamingAssets/Osm/berlin.osm.xml";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtyMap Configuration")
	FString MapCssFileAsString = "C:/workspace/Projects/UtymapUnreal/utymap/unity/demo/Assets/StreamingAssets/MapCss/default/default.mapcss";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtyMap Configuration")
	FString IndexFolder = "C:/workspace/Projects/UtymapUnreal/utymap/unity/demo/Assets/StreamingAssets/Index/";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtyMap Configuration")
	EElevationDataType ElevationType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtyMap Configuration")
	FGeoCoordinate StartPosition = FGeoCoordinate(52.53171, 13.38721);

	// Level Of Detail of the map to load.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtyMap Configuration", meta = (ClampMin = "1", ClampMax = "16", UIMin = "1", UIMax = "16"))
		int32 LevelOfDetail = 16;

	// Centimeters the player has to move to trigger query an map update
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtyMap Configuration", meta = (ClampMin = "100", ClampMax = "10000", UIMin = "100", UIMax = "10000"))
	float PositionUpdateThreshold = 100;

private:
	UPROPERTY()
	ATileController* TileController;
	UPROPERTY()
	UMapDataLoader* MapDataLoader;
	UPROPERTY()
	UCartesianProjection* Projection;
	UPROPERTY()
	UStylesheet* Stylesheet;
	UPROPERTY()
	UOpenStreetMapDataProvider* OpenStreetMapProvider;
	UPROPERTY()
	ADemoModelBuilder* ModelBuilder;

	FGeoCoordinate GetWorldZeroPoint() { return StartPosition; }

	APlayerController* PlayerController;
	APawn* PlayerPawn;

	FVector LastKnownPlayerPosition;
};
