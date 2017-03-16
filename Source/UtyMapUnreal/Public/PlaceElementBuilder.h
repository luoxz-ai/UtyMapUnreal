#pragma once

#include "GameFramework/Actor.h"
#include "CustomizationService.h"
#include "PlaceElementBuilder.generated.h"

UCLASS()
class UTYMAPUNREAL_API APlaceElementBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	APlaceElementBuilder();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	void Build(ATile* tile, FElement& element);
	void SetActiveWorld(UWorld* World);
	void SetCustomizationService(UCustomizationService* customizationService);

private:
	UCustomizationService* _customizationService;
	UWorld* ActiveWorld;
	int UnnamedElementIndex;

	UMaterialInterface* GetMaterial(FElement& element);
	TArray<FVector2D> GetUV(FElement& element);
	FRectangle GetUvRect(FElement& element);
	FString GetName(FElement& element);
	float GetMinHeight(FElement& element);
};
