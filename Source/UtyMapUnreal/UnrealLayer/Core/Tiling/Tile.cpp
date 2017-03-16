#include "UtyMapUnreal.h"
#include "Tile.h"
#include "GeoUtils.h"
#include <sstream> 
#include <string>
//#include "RuntimeMeshComponent.h"




TSet<long long>* const  ATile::GlobalIds = new TSet<long long>();

ATile::~ATile()
{
	// remove all registered ids from global list if they are in current registry
	for (auto id : *_localIds)
	{
		GlobalIds->Remove(id);
	}
	_localIds->Empty();
}

URuntimeMeshComponent * ATile::GetMeshComponent()
{
	return Meshes;
}

void ATile::SetMeshComponent(URuntimeMeshComponent* NewMeshComp)
{
	Meshes = NewMeshComp;
}

URuntimeMeshComponent* ATile::GetElementComponent()
{
	return ElementsMeshes;
}

void ATile::SetElementComponent(URuntimeMeshComponent* NewMeshComp)
{
	if (NewMeshComp)
	{
		ElementsMeshes = NewMeshComp;
	}
}

void ATile::AddRuntimeMeshComponent(AMyMesh* NewMeshComp)
{
	if (NewMeshComp)
	{
		MeshComponents.Add(NewMeshComp);
	}
}

FRectangle* ATile::GetRectangle() const
{
	return Rectangle;
}

void ATile::SetRectangle(FRectangle* value)
{
	Rectangle = value;
}

FBoundingBox* ATile::GetBoundingBox() const
{
	return BoundingBox;
}

void ATile::SetBoundingBox(FBoundingBox* value)
{
	BoundingBox = value;
}

FQuadKey* ATile::GetQuadKey() const
{
	return QuadKey;
}

void ATile::SetQuadKey(FQuadKey* value)
{
	QuadKey = value;
}

UStylesheet* ATile::GetStylesheet() const
{
	return Stylesheet;
}

void ATile::SetStylesheet(UStylesheet* value)
{
	Stylesheet = value;
}

UCartesianProjection* ATile::GetProjection() const
{
	return Projection;
}

void ATile::SetProjection(UCartesianProjection* value)
{
	Projection = value;
}

void ATile::Init(FQuadKey quadKey, UStylesheet* stylesheet, UCartesianProjection* projection)
{
	FQuadKey* newQuadKey = new FQuadKey(quadKey);
	SetQuadKey(newQuadKey);
	SetStylesheet(stylesheet);
	SetProjection(projection);

	SetBoundingBox(FGeoUtils::QuadKeyToBoundingBox(quadKey));
	SetRectangle(FGeoUtils::QuadKeyToRect(projection, quadKey));

	// todo: init gameobject UObject here / for convenience maybe an actor that can be selected and seen in the editor
	// tdoo: look at demo project, as the building is done there. it implements a IModelBuilder
}

ATile::ATile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Setup render component
	Meshes = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("Meshes"));
	ElementsMeshes = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("ElementMeshes"));
	TileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TileRoot"));

	// make a sceneroot for convienience in scene outliner
	RootComponent = TileRoot;
	Meshes->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ElementsMeshes->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

bool ATile::Contains(FVector2D* position, double offset)
{
	return (position->X > GetRectangle()->GetLeft() + offset) && (position->Y < GetRectangle()->GetTop() - offset) && (position->X < GetRectangle()->GetRight()- offset) && (position->Y > GetRectangle()->GetBottom() + offset);
}

std::string ATile::ToString()
{
	std::stringstream ss;
	ss << "Tile " << QuadKey->TileX << "," << QuadKey->TileY << ":" << QuadKey->LevelOfDetail;
	return ss.str();
}

bool ATile::Has(long long id)
{
	return GlobalIds->Contains(id);
}

void ATile::Register(long long id)
{
	_localIds->Add(id);
	GlobalIds->Add(id);
}
