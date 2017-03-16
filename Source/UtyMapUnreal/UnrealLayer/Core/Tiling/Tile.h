#pragma once

#include <string>
#include "Set.h"
#include "Rectangle.h"
#include "BoundingBox.h"
#include "Stylesheet.h"
#include "IProjection.h"
#include "QuadKey.h"
#include "RuntimeMeshComponent.h"
#include "RuntimeMeshObjectBase.h"
#include "Tile.generated.h"


UCLASS()
class UTYMAPUNREAL_API AMyMesh : public ARuntimeMeshObjectBase
{
	GENERATED_BODY()

public:
	AMyMesh() {};

};

/**
 * Map Tile representation - has a RuntimeMeshComponent that that holds all elements to renderActor
 */
UCLASS(BlueprintType, Blueprintable)
class ATile : public AActor
{
	GENERATED_BODY()

public:
	ATile(const FObjectInitializer& ObjectInitializer);

private:
	FRectangle* Rectangle;
	FBoundingBox* BoundingBox;
	FQuadKey* QuadKey;
	UStylesheet* Stylesheet;
	UCartesianProjection* Projection;
	// Sets game object which holds all children objects.

	int32 FirstFreeMeshSectionIndex = 0;
	int32 FirstFreeElementSectionIndex = 0;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
	URuntimeMeshComponent* Meshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
	URuntimeMeshComponent* ElementsMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
	TArray<AMyMesh*> MeshComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
	USceneComponent* TileRoot;


	int32 GetMeshSectionIndex() { return FirstFreeMeshSectionIndex; }
	void IncrementMeshSectionIndex() { FirstFreeMeshSectionIndex++;	}
	int32 GetElementSectionIndex() { return FirstFreeElementSectionIndex; }
	void IncrementElementSectionIndex() { FirstFreeElementSectionIndex++; }

	// can modify meshcomponent - called from meshBuilder
	URuntimeMeshComponent* GetMeshComponent();
	void SetMeshComponent(URuntimeMeshComponent* NewMeshComp);
	// can modify element scene component - called from ElementMeshBuilder
	URuntimeMeshComponent* GetElementComponent();
	void SetElementComponent(URuntimeMeshComponent* NewMeshComp);

	void AddRuntimeMeshComponent(AMyMesh* NewMeshComp);


	// Tile rectamgle in world coordinates.
	FRectangle* GetRectangle() const;
	void SetRectangle(FRectangle* value);

	// Tile geo bounding box.
	FBoundingBox* GetBoundingBox() const;
	void SetBoundingBox(FBoundingBox* value);

	// Corresponding quadkey.
	FQuadKey* GetQuadKey() const;
	void SetQuadKey(FQuadKey*  value);

	// Stylesheet.
	UStylesheet* GetStylesheet() const;
	void SetStylesheet(UStylesheet* value);

	// Used projection.
	UCartesianProjection* GetProjection() const;
	void SetProjection(UCartesianProjection* value);


private:
	// Stores element ids loaded in this tile.
	TSet<long long>* const _localIds = new TSet<long long>();

	// Stores element ids loaded for all tiles.
	static TSet<long long>* const  GlobalIds;

public:
	// Creates <see cref="Tile"/>.
	void Init(FQuadKey quadKey, UStylesheet* stylesheet, UCartesianProjection* projection);

	// Checks whether absolute position locates in tile with bound offset.
	// @param position">Absolute position in game
	// @param offset">offset from bounds
	// @return Tres if position in tile</returns>
	bool Contains(FVector2D* position, double offset);

	
	virtual std::string ToString();

	// Checks whether element with specific id is registered.
	// @param id">Element id
	// @return True if registration is found
	bool Has(long long id);

	// Register element with given id inside to prevent multiple loading.
	void Register(long long id);


	
	~ATile();

};
