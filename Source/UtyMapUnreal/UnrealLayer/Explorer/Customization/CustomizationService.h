#pragma once

#include "Materials/Material.h"
#include <string>
#include "AssetData.h"
#include "Array.h"
#include "Engine/StreamableManager.h"
#include "CustomizationService.generated.h"

UCLASS()
class UCustomizationService : public UObject
{
	GENERATED_BODY()

public:
	// Gets shared material by key. 
	// @Remark: Should be called from Game thread only
	UMaterialInstanceDynamic* GetSharedMaterial(const std::string& key);

	bool ListAllBlueprintsInPath(FName Path, TArray<UClass*>& Result, UClass* Class);
	UCustomizationService(const FObjectInitializer& ObjectInitializer);

private:
	//TMap<FString, UMaterial*> _modelBehaviours;
	TMap<FString, UMaterial*> _sharedMaterials; 
	//FAssetData FindMaterial(const std::string& key);
	//void RequestLoad(TArray<FStringAssetReference>& ItemsToStream);
	//void OnLoadComplete();
	//UObjectLibrary* ObjectLibrary;
	//TArray<FAssetData> AssetDatas;
	//FStreamableManager Streamable;
	//TArray<FStringAssetReference> LoadRequestedMaterials;
	TArray<UMaterial*> LoadedMaterials;
	UMaterialInstanceDynamic* MaterialInstance;
	UMaterialInstanceDynamic* MaterialInstanceIcons;
	UMaterial*	TheMaterial;
	UMaterial* TheIconMaterial;
};
