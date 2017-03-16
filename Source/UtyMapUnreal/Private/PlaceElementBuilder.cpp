#include "UtyMapUnreal.h"
#include "PlaceElementBuilder.h"
#include "Components/BoxComponent.h"
#include "Rectangle.h"
#include "Element.h"
#include "Array.h"
#include "UnrealString.h"
#include "helpers/string_helpers.hpp"
#include "RuntimeMeshLibrary.h"
#include <stdlib.h>

APlaceElementBuilder::APlaceElementBuilder()
{
	PrimaryActorTick.bCanEverTick = false;

	_customizationService = CreateDefaultSubobject<UCustomizationService>(TEXT("MaterialLoadingService"));
}

void APlaceElementBuilder::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlaceElementBuilder::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void APlaceElementBuilder::SetActiveWorld(UWorld* World)
{
	ActiveWorld = World;
}

void APlaceElementBuilder::SetCustomizationService(UCustomizationService* customizationService)
{
	_customizationService = customizationService;
}

void APlaceElementBuilder::Build(ATile* tile, FElement& element)
{
	// TODO: disabled during mesh uv problem solviing
	return;

	if (!tile->IsValidLowLevel())
	{
		UE_LOG(UtyProcessing, Error, TEXT("Tile invalid when spawning an map element"));
		return;
	}
	
	if (ActiveWorld)
	{
		if (!ActiveWorld->IsValidLowLevel())
		{
			UE_LOG(UtyProcessing, Error, TEXT("No unreal world present"));
			return;
		}
	}

	std::string name;
	if (!element.Tags["name"].size())
	{
		char IndexString[30];
		name = _itoa_s(UnnamedElementIndex++, IndexString, 10);
	}
	else
	{
		name = element.Tags["name"].c_str();
	}

	FTransform transform = tile->GetActorTransform();
	transform.SetLocation(*(tile->GetProjection()->Project(element.Geometry[0],
		GetMinHeight(element) + element.Heights[0])));
	transform.SetScale3D(FVector(2, 2, 2));

	// mesh section
	TArray<FVector> VerticesUnreal;
	TArray<int32> TrianglesUnreal;
	TArray<FColor> ColorsUnreal;
	TArray<FVector> NormalsUnreal;
	TArray<FRuntimeMeshTangent> TangentsUnreal;
	TArray<FVector2D> UVsUnreal; 

	URuntimeMeshLibrary::CreateBoxMesh(FVector(100), VerticesUnreal, TrianglesUnreal, NormalsUnreal, UVsUnreal, TangentsUnreal);

	//UVsUnreal = GetUV(element);
	tile->GetElementComponent()->CreateMeshSection(
		tile->GetElementSectionIndex(),
		VerticesUnreal,
		TrianglesUnreal,
		NormalsUnreal,
		UVsUnreal,
		UVsUnreal,
		ColorsUnreal,
		TangentsUnreal,
		false,
		EUpdateFrequency::Infrequent,
		ESectionUpdateFlags::None
	);
	tile->GetElementComponent()->SetMaterial(tile->GetElementSectionIndex(), GetMaterial(element));
	tile->IncrementElementSectionIndex();

	//UE_LOG(UtyProcessing, Warning, TEXT("Created element %s"), UTF8_TO_TCHAR(name.c_str()));
}

TArray<FVector2D> APlaceElementBuilder::GetUV(FElement& element)
{
	
	FRectangle rect = GetUvRect(element);

	// todo: properly map unity to unreal texture coordinates here
	/*auto p0 = new FVector2D(rect.xMin, rect.yMin);
	auto p1 = new FVector2D(rect.xMax, rect.yMin);
	auto p2 = new FVector2D(rect.xMin, rect.yMax);
	auto p3 = new FVector2D(rect.xMax, rect.yMax);
*/
	// Imagine looking at the front of the cube, the first 4 vertices are arranged like so
	//   2 --- 3
	//   |     |
	//   |     |
	//   0 --- 1
	// then the UV's are mapped as follows
	//    2    3    0    1   Front
	//    6    7   10   11   Back
	//   19   17   16   18   Left
	//   23   21   20   22   Right
	//    4    5    8    9   Top
	//   15   13   12   14   Bottom
	TArray<FVector2D> UVs;
	return UVs; /* = {
		p0, p1, p2, p3,
		p2, p3, p2, p3,
		p0, p1, p0, p1,
		p0, p3, p1, p2,
		p0, p3, p1, p2,
		p0, p3, p1, p2 
	};
	*/
}

FString APlaceElementBuilder::GetName(FElement& element)
{
	return FString(UTF8_TO_TCHAR(element.Tags["name"].c_str()));
		//element.Tags.Aggregate("", (s, t) = > s += FString.Format("{0}={1},", TArray<FStringFormatArg> {t.Key, t.Value}))});*/
}

float APlaceElementBuilder::GetMinHeight(FElement& element)
{
	return element.Styles.find("min-height") != element.Styles.end()
		? std::atof(element.Styles["min-height"].c_str())
		: 0.0f;
}

UMaterialInterface* APlaceElementBuilder::GetMaterial(FElement& element)
{
	if (_customizationService)
	{
		return _customizationService->GetSharedMaterial("Materials/" + element.Styles["material"]);
	}

	UE_LOG(UtyProcessing, Error, TEXT("Material provider invalid."));
	return nullptr;
}

FRectangle APlaceElementBuilder::GetUvRect(FElement& element)
{
	auto values = split(element.Styles["rect"], '_');
	if (values.size() != 4)
	{
		UE_LOG(UtyProcessing, Error, TEXT("Cannot read uv mapping."));
		return FRectangle(0, 0, 0, 0);
	}

	auto textureHeight = std::stof(element.Styles["height"]);
	auto textureWidth = std::stof(element.Styles["width"]);

	auto width = static_cast<float>(std::stoi(values[2]));
	auto height = static_cast<float>(std::stoi(values[3]));

	auto offset = std::stoi(values[1]);
	auto x = static_cast<float>(std::stoi(values[0]));
	auto y = std::abs((offset + height) - textureHeight);

	// uv mapping in unreal is different from unity u is the same, v is leftTop  (or 1-y)
	auto leftBottom = new FVector2D(x / textureWidth, 1 - (y / textureHeight));
	auto rightUpper = new FVector2D((x + width) / textureWidth, 1 - ((y + height) / textureHeight));

	return FRectangle(leftBottom->X, leftBottom->Y, rightUpper->X - leftBottom->X, rightUpper->Y - leftBottom->Y);
}