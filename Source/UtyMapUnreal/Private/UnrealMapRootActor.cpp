#include "UtyMapUnreal.h"
#include "UnrealMapRootActor.h"
#include "UnrealLayer/Maps/Data/MapDataLoader.h"

AUnrealMapRootActor::AUnrealMapRootActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void AUnrealMapRootActor::BeginPlay()
{
	Super::BeginPlay();

	// update GPS position if set by user
	StartPosition.Latitude = StartPosition.Latitude;
	StartPosition.Longitude = StartPosition.Longitude;
	
	Projection = NewObject<UCartesianProjection>(this, TEXT("CartesianProjection"), RF_Transactional);
	Projection->Init(GetWorldZeroPoint());
	Stylesheet = NewObject<UStylesheet>(this, TEXT("Stylesheet"), RF_Transactional);
	// todo: make this an drag and drop asset like streetmap osm by Mike Fricker does it, or error protect somehow
	Stylesheet->SetFilename(std::string(TCHAR_TO_UTF8(*MapCssFileAsString)));

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ModelBuilder = GetWorld()->SpawnActor<ADemoModelBuilder>(FVector(0, 0, 0), FRotator(0, 90, 0),SpawnParams);
	if (!ModelBuilder)
	{
		UE_LOG(UtyInit, Error, TEXT("ModelBuilder invalid, setup aborted!"));
		return;
	}
	ModelBuilder->SetActiveWorld(GetWorld());
	
	TileController = GetWorld()->SpawnActor<ATileController>(FVector(0, 0, 0), FRotator());
	TileController->Init();
	
	MapDataLoader = NewObject<UMapDataLoader>(this,TEXT("MapDataLoader"), RF_Transactional);
	
	OpenStreetMapProvider = NewObject<UOpenStreetMapDataProvider>(this, TEXT("OpenStreetMapModule"), RF_Transactional);

	if (!MapDataLoader->IsValidLowLevel() || !TileController->IsValidLowLevel())
	{
		UE_LOG(UtyInit, Error, TEXT("MapDataLoader invalid, setup aborted!"))
	}

	MapDataLoader->Init(ModelBuilder, OpenStreetMapProvider, TCHAR_TO_UTF8(*IndexFolder), TCHAR_TO_UTF8(*MapDataFileAsString), Stylesheet->GetPath().c_str(),  ElevationType, TLodRange<int32>(LevelOfDetail, LevelOfDetail));
	TileController->Init(Projection, Stylesheet, MapDataLoader);

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	PlayerController = GetWorld()->GetFirstPlayerController();

	LastKnownPlayerPosition = PlayerPawn->GetActorLocation();
	// triggers initial load of the underlying map
	UpdatedPosition.Broadcast(LastKnownPlayerPosition, LevelOfDetail);
}


void AUnrealMapRootActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (MapDataLoader)
	{
		MapDataLoader->Shutdown();
	}
}

void AUnrealMapRootActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (!PlayerPawn)
	{
		return;
	}

	// Watch for Player movement and send an update to the TileController in case it changed more than PositionUpdateThreshold
	if (FVector::Dist(LastKnownPlayerPosition, PlayerPawn->GetActorLocation()) > PositionUpdateThreshold)
	{
		LastKnownPlayerPosition  = PlayerPawn->GetActorLocation();
		UpdatedPosition.Broadcast(LastKnownPlayerPosition, LevelOfDetail);
	}

	MapDataLoader->FetchAndResetOutstandingArrivedElements();
}

