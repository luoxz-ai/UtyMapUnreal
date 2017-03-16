// Fill out your copyright notice in the Description page of Project Settings.

#include "UtyMapUnreal.h"
#include "TileController.h"
#include "Public/UnrealMapRootActor.h"

#include "TileController.h"
#include "../Utils/GeoUtils.h"

using namespace UtyDepend;
using namespace UtyDepend::Config;



ATileController::ATileController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) //, MyObjectInitializer(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

ATileController::~ATileController()
{
	UE_LOG(UtyInit, Warning, TEXT("TileController destroyed"));
}

void ATileController::BeginPlay()
{
	// register ourself for Position update events from root actor (that watches for player pawn movement)
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnrealMapRootActor::StaticClass(), FoundActors);

	AUnrealMapRootActor* Root = Cast<AUnrealMapRootActor>(FoundActors[0]);
	if (Root)
	{
		Root->UpdatedPosition.AddDynamic(this, &ATileController::OnPositionUpdate);
		//OnTileLoadResponse.AddDynamic(this, &ATileController::OnTileLoadFinished);
	}
}

void ATileController::Tick(float DeltaSeconds)
{
	UE_LOG(UtyInit, Warning, TEXT("TileController tick"));
}

void ATileController::OnPositionUpdate(FVector Position, int32 LevelOfDetail)
{
	//UE_LOG(UtyInit, Warning, TEXT("TileController:  position received"));
	FVector NewPosition = Position;
	OnPosition(Projection->Project(&NewPosition), LevelOfDetail);
}

UStylesheet* ATileController::GetStylesheet() const
{
	return Stylesheet;
}

void ATileController::SetStylesheet(UStylesheet* value)
{
	Stylesheet = value;
}

UCartesianProjection* ATileController::GetProjection() const
{
	return Projection;
}

void ATileController::SetProjection(UCartesianProjection* value)
{
	Projection = value;
}

void ATileController::OnPosition(FGeoCoordinate coordinate, int levelOfDetails)
{
	OnPosition(coordinate, Projection->Project(coordinate, 0), levelOfDetails);
}

void ATileController::OnRegion(FRectangle* boundaries, int levelOfDetails)
{
	OnRegion(FGeoUtils::RectToBoundingBox(Projection, boundaries), levelOfDetails);
}

void ATileController::OnRegion(FBoundingBox* boundaries, int levelOfDetails)
{
	auto BoundariesAsQuadKey = FGeoUtils::BoundingBoxToQuadKeys(boundaries, levelOfDetails);
	OnRegion(BoundariesAsQuadKey);
}

void ATileController::OnRegion(std::vector<FQuadKey>& quadKeys)
{
	{
		std::lock_guard<std::mutex> lock(_lockObj);
		for (std::vector<FQuadKey>::const_iterator quadKey = quadKeys.begin(); quadKey != quadKeys.end(); ++quadKey)
		{
			if (_loadedTiles.find(*quadKey) == _loadedTiles.end())
			{
				Load(*quadKey);
			}
		}
	}
}

void ATileController::OnPosition(FGeoCoordinate geoPosition, FVector* position, int levelOfDetails)
{
	// call update logic only if threshold is reached
	if (FVector::Dist(*position, *_lastUpdatePosition) > _moveSensitivity || _currentQuadKey.LevelOfDetail != levelOfDetails)
	{
		{
			std::lock_guard<std::mutex> lock(_lockObj);
			_lastUpdatePosition = position;
			_currentQuadKey = FGeoUtils::CreateQuadKey(geoPosition, levelOfDetails);

			UnloadFarTiles(_currentQuadKey);

			if (_loadedTiles.find(_currentQuadKey) != _loadedTiles.end())
			{
				auto TileEntry = _loadedTiles.find(_currentQuadKey);
				auto FoundTile = TileEntry->second;
				if (ShouldPreload(FoundTile, position))
				{
					PreloadNextTile(FoundTile, position);
				}
				return;
			}

			Load(_currentQuadKey);
		}
	}
}

void ATileController::Init()
{
	_moveSensitivity = 30; // if set too high disables execution of loading as the distance is also part of the player movement update event!
	_offsetRatio = 10; // percentage of tile size
	_maxTileDistance = 2;
}

void ATileController::Init(UCartesianProjection* ProjectionIn, UStylesheet* StylesheetIn, UMapDataLoader* MapDataLoader)
{
	SetProjection(ProjectionIn);
	SetStylesheet(StylesheetIn);
	OnTileLoadRequest.AddDynamic(MapDataLoader, &UMapDataLoader::Load);
}

//
//void ATileController::OnTileLoadFinished(ATile* tile)
//{
//	int32 FoundIndex = INDEX_NONE;
//	if (FoundIndex = OpenTileLoadRequests.Find(tile))
//	{
//		OpenTileLoadRequests.Remove(FoundIndex);
//	}
//	else
//	{
//		UE_LOG(UtyProcessing, Warning, TEXT("Tilecontroller: Load finished received for a tile we didn't request. Ignoring!"));
//	}
//
//}

void ATileController::Configure(IConfigSection* configSection)
{
	_moveSensitivity = configSection->GetFloat(std::string(R"(tile/sensitivity)"), 30);
	_offsetRatio = configSection->GetFloat(std::string(R"(tile/offset)"), 10); // percentage of tile size
	_maxTileDistance = configSection->GetInt(std::string(R"(tile/max_tile_distance)"), 2);
}

void ATileController::Load(FQuadKey quadKey)
{
	if (_loadedTiles.size())
	{
		if (_loadedTiles.find(quadKey) != _loadedTiles.end())
		{
			UE_LOG(UtyProcessing, Warning, TEXT("Tile already loaded"));
			return;
		}
	}
	auto tile = GetWorld()->SpawnActor<ATile>();
	tile->Init(quadKey, Stylesheet, Projection);
	auto pair = std::make_pair(quadKey, tile);
	_loadedTiles.insert(pair);
	_loadedTilesUnreal.Add(tile);

	SendTileLoadRequest(tile);
}

void ATileController::PreloadNextTile(ATile* tile, FVector* position)
{
	auto quadKey = GetNextQuadKey(tile, position);
	if (_loadedTiles.find(quadKey) == _loadedTiles.end())
	{
		Load(quadKey);
	}
}

bool ATileController::ShouldPreload(ATile* tile, FVector* position)
{
	auto ProjectedPosition = new FVector2D(position->X, position->Y); // Todo: check if it works as intended
	return !tile->Contains(ProjectedPosition, tile->GetRectangle()->GetWidth() * _offsetRatio);
}

FQuadKey ATileController::GetNextQuadKey(ATile* tile, FVector* position)
{
	auto quadKey = tile->GetQuadKey();
	auto position2D = new FVector2D(position->X, position->Y); // Todo: changed Z to Y, check if ok

															   // NOTE left-right and top-bottom orientation
	FVector2D* topLeft = new FVector2D(tile->GetRectangle()->GetLeft(), tile->GetRectangle()->GetTop());
	FVector2D* topRight = new FVector2D(tile->GetRectangle()->GetRight(), tile->GetRectangle()->GetTop());

	// top
	if (IsPointInTriangle(position2D, tile->GetRectangle()->GetCenter(), topLeft, topRight))
	{
		return FQuadKey(quadKey->TileX, quadKey->TileY - 1, quadKey->LevelOfDetail);
	}

	FVector2D* bottomLeft = new FVector2D(tile->GetRectangle()->GetLeft(), tile->GetRectangle()->GetBottom());

	// left
	if (IsPointInTriangle(position2D, tile->GetRectangle()->GetCenter(), topLeft, bottomLeft))
	{
		return FQuadKey(quadKey->TileX - 1, quadKey->TileY, quadKey->LevelOfDetail);
	}

	FVector2D* bottomRight = new FVector2D(tile->GetRectangle()->GetRight(), tile->GetRectangle()->GetBottom());

	// right
	if (IsPointInTriangle(position2D, tile->GetRectangle()->GetCenter(), topRight, bottomRight))
	{
		return FQuadKey(quadKey->TileX + 1, quadKey->TileY, quadKey->LevelOfDetail);
	}

	// bottom
	return FQuadKey(quadKey->TileX, quadKey->TileY + 1, quadKey->LevelOfDetail);
}

void ATileController::UnloadFarTiles(FQuadKey currentQuadKey)
{
	auto tiles = _loadedTiles;//.ToArray();

	for (auto loadedTile = tiles.begin(); loadedTile != tiles.end(); ++loadedTile)
	{
		auto quadKey = (*loadedTile).first;
		if ((std::abs(quadKey.TileX - currentQuadKey.TileX) + std::abs(quadKey.TileY - currentQuadKey.TileY)) <= _maxTileDistance)
		{
			continue;
		}

		ATile* TileRef = (*loadedTile).second;

		TileRef->MarkPendingKill();
		UE_LOG(UtyProcessing, Warning, TEXT("Unloading tile %s"),UTF8_TO_TCHAR(TileRef->ToString().c_str()));
		_loadedTilesUnreal.Remove(TileRef);
		//delete (*loadedTile).second;
		_loadedTiles.erase(quadKey);

		// todo maange deletion event in rest of system
		//NotifyOnNextObservers((*loadedTile).second);
	}
}

void ATileController::Subscribe(AActor* Caller)
{
	//if (Caller)
	//{
	//	OnTileLoadRequest.AddDynamic(Caller,&
	//}
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnrealMapRootActor::StaticClass(), FoundActors);

	//AUnrealMapRootActor* Root = Cast<AUnrealMapRootActor>(FoundActors[0]);
	//if (Root)
	//{
	//	Root->UpdatedPosition.AddDynamic(this, &ATileController::OnPositionUpdate);
	//}
}

void ATileController::SendTileLoadRequest(ATile* tile)
{
	OnTileLoadRequest.Broadcast(tile);
	OpenTileLoadRequests.Add(tile);
}

bool ATileController::IsPointInTriangle(FVector2D* p, FVector2D* p1, FVector2D* p2, FVector2D* p3)
{
	auto alpha = ((p2->Y - p3->Y) * (p->X - p3->X) + (p3->X - p2->X) * (p->Y - p3->Y)) / ((p2->Y - p3->Y) * (p1->X - p3->X) + (p3->X - p2->X) * (p1->Y - p3->Y));
	auto beta = ((p3->Y - p1->Y) * (p->X - p3->X) + (p1->X - p3->X) * (p->Y - p3->Y)) / ((p2->Y - p3->Y) * (p1->X - p3->X) + (p3->X - p2->X) * (p1->Y - p3->Y));
	auto gamma = 1.0f - alpha - beta;

	return alpha > 0 && beta > 0 && gamma > 0;
}
//
//UTileController::~UTileController()
//{
//	for (auto tile : _loadedTiles)
//	{
//		delete tile.second;
//	}
//	/*for (std::vector<IObserver<FTile*>*>::const_iterator tileObserver = _tileObservers.begin(); tileObserver != _tileObservers.end(); ++tileObserver)
//	{
//		(*tileObserver)->OnCompleted();
//	}*/
//}