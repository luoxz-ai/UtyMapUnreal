#pragma once

#include "GameFramework/Actor.h"
#include "Tile.h"
#include "Element.h"
#include "Mesh.h"
#include "CustomizationService.h"
#include "PlaceElementBuilder.h"
#include "RuntimeMeshObjectBase.h"
#include "DemoModelBuilder.generated.h"

UCLASS()
class UTYMAPUNREAL_API ADemoModelBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	ADemoModelBuilder();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "MyProject")
	void BuildElement(ATile* tile, FElement& element);
	UFUNCTION(BlueprintCallable, Category = "MyProject")
	void BuildMesh(ATile* tile, FMesh& mesh);

	void SetActiveWorld(UWorld* World);

private:
	void EnsureTile(ATile* tile);
	APlaceElementBuilder* _placeElementBuilder;
	UCustomizationService* _customizationService;
	UWorld* ActiveWorld; 
};
