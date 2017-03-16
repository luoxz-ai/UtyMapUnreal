// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Rectangle.h"
#include "IConfigSection.h"
#include "Stylesheet.h"
#include "IProjection.h"
#include "BoundingBox.h"
#include "Tile.h"
#include "GeoCoordinate.h"
#include "QuadKey.h"
#include "IConfigurable.h"
#include <unordered_map>
#include <vector>
#include <cmath>
#include <limits>
#include <utility>
#include <mutex>
#include <functional>
#include "MapDataLoader.h"
#include "TileController.generated.h"


using namespace UtyDepend;
using namespace UtyDepend::Config;


// requests loading of a new tile
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTileLoadingRequest, ATile*, TileToLoad);
//
//// response loading of a new tile
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTileLoadingComplete, ATile*, TileLoaded);


/**
* TileController holds the tiles and requests loading of tiles not in memory storage
*/
UCLASS()
class ATileController : public AActor
{
	GENERATED_BODY()

public:
	ATileController(const FObjectInitializer& ObjectInitializer);
	~ATileController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/**
	* Checks if tile is loaded for that region and loads if not
	*/
	UFUNCTION(BlueprintCallable, Category = "MyProject")
		void OnPositionUpdate(FVector Position, int32 LevelOfDetail);

	void Init();
	void Configure(IConfigSection* configSection);

	UPROPERTY(BlueprintAssignable)
	FTileLoadingRequest OnTileLoadRequest;

	void Init(UCartesianProjection* ProjectionIn, UStylesheet* StylesheetIn, UMapDataLoader* MapDataLoader);

/*
	UPROPERTY(BlueprintAssignable)
	FTileLoadingComplete OnTileLoadResponse;

	UFUNCTION(BlueprintCallable, Category = "MyProject")
	void OnTileLoadFinished(ATile* tile);
*/
private:
	UStylesheet *Stylesheet;
	UCartesianProjection *Projection;

	std::mutex _lockObj;

	double _offsetRatio = 0;
	double _moveSensitivity = 0;
	int _maxTileDistance = 0;

	FVector* _lastUpdatePosition = new FVector(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

	FQuadKey _currentQuadKey;

	struct FQuadKeyHashing {
		size_t operator()(const FQuadKey &name) const
		{
			int hash = 17;
			hash = hash * 29 + std::hash<int>()(name.TileX);
			hash = hash * 29 + std::hash<int>()(name.TileY);
			hash = hash * 29 + std::hash<int>()(name.LevelOfDetail);
			return hash;
		}
	};

	std::unordered_map<FQuadKey, ATile*, FQuadKeyHashing> _loadedTiles;
	// ensures proper destruction after usage by putting into a unreal data structure
	UPROPERTY()
	TArray<ATile*> _loadedTilesUnreal;
	UPROPERTY()
	TArray<ATile*> OpenTileLoadRequests;

public:
	UStylesheet* GetStylesheet() const;
	void SetStylesheet(UStylesheet* value);

	UCartesianProjection* GetProjection() const;
	void SetProjection(UCartesianProjection* value);

	void OnPosition(FGeoCoordinate coordinate, int levelOfDetails);

	void OnRegion(FRectangle* boundaries, int levelOfDetails);

	void OnRegion(FBoundingBox* boundaries, int levelOfDetails);

	void OnRegion(std::vector<FQuadKey>& quadKeys);

private:
	void OnPosition(FGeoCoordinate geoPosition, FVector* position, int levelOfDetails);

	// Loads tile for given quadKey.
	UFUNCTION(BlueprintCallable, Category = "MyProject")
	void Load(FQuadKey quadKey);

	void PreloadNextTile(ATile* tile, FVector* position);

	bool ShouldPreload(ATile* tile, FVector* position);

	// Gets next quadkey.
	FQuadKey GetNextQuadKey(ATile* tile, FVector* position);

	// Removes far tiles from list of loaded and sends corresponding message.
	void UnloadFarTiles(FQuadKey currentQuadKey);

	void Subscribe(AActor* Caller);

	void SendTileLoadRequest(ATile* tile);

	/** Checks whether point is located in triangle.
	*   @note http://stackoverflow.com/questions/13300904/determine-whether-point-lies-inside-triangle
	*/
	static bool IsPointInTriangle(FVector2D* p, FVector2D* p1, FVector2D* p2, FVector2D* p3);

};
