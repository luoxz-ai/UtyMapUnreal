#include "UtyMapUnreal.h"
#include "CustomizationService.h"
#include "Materials/Material.h"
#include "Casts.h"

////TEMPLATE Load Obj From Path
//template <typename ObjClass>
//static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
//{
//	if (Path == NAME_None) return NULL;
//	//~
//
//	return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
//}
//
//// Load Material From Path 
//static FORCEINLINE UMaterialInterface* LoadMatFromPath(const FName& Path)
//{
//	if (Path == NAME_None) return NULL;
//	//~
//
//	return LoadObjFromPath<UMaterialInterface>(Path);
//}
//
//bool UCustomizationService::ListAllBlueprintsInPath(FName Path, TArray<UClass*>& Result, UClass* Class)
//{
//	auto Library = UObjectLibrary::CreateLibrary(Class, true, GIsEditor);
//	Library->LoadBlueprintAssetDataFromPath(Path.ToString());
//
//	TArray<FAssetData> Assets;
//	Library->GetAssetDataList(Assets);
//
//	for (auto& Asset : Assets)
//	{
//		UBlueprint* bp = Cast<UBlueprint>(Asset.GetAsset());
//		if (bp)
//		{
//			auto gc = bp->GeneratedClass;
//			if (gc)
//			{
//				Result.Add(gc);
//			}
//		}
//		else
//		{
//			auto GeneratedClassName = (Asset.AssetName.ToString() + "_C");
//
//			UClass* Clazz = FindObject<UClass>(Asset.GetPackage(), *GeneratedClassName);
//			if (Clazz)
//			{
//				Result.Add(Clazz);
//			}
//			else
//			{
//				UObjectRedirector* RenamedClassRedirector = FindObject<UObjectRedirector>(Asset.GetPackage(), *GeneratedClassName);
//				if (RenamedClassRedirector)
//				{
//					Result.Add(CastChecked<UClass>(RenamedClassRedirector->DestinationObject));
//				}
//			}
//
//		}
//	}
//
//	return true;
//}

UCustomizationService::UCustomizationService(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), _sharedMaterials(TMap<FString, UMaterial*>())
{
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/UtyMap/Materials/M_ShadedColor.M_ShadedColor'"));
	TheMaterial = nullptr;
	MaterialInstance = nullptr;
	if (Material.Object != NULL)
	{
		TheMaterial = (UMaterial*)Material.Object;
	}
	if (TheMaterial)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(TheMaterial, this, FName("TextureAtlas"));
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialIcons(TEXT("Material'/Game/UtyMap/Materials/M_Info_Icons.M_Info_Icons'"));
	TheIconMaterial = nullptr;
	MaterialInstanceIcons = nullptr;
	if (MaterialIcons.Object != NULL)
	{
		TheIconMaterial = (UMaterial*)MaterialIcons.Object;
	}
	if (TheIconMaterial)
	{
		MaterialInstanceIcons = UMaterialInstanceDynamic::Create(TheIconMaterial, this, FName("Icons"));
	}

	//TArray<UClass*> Lol;
	//ListAllBlueprintsInPath(FName(TEXT("/Game/StarterContent/Materials/")), Lol, UMaterial::StaticClass());

	//if (!ObjectLibrary)
	//{
	//	ObjectLibrary = UObjectLibrary::CreateLibrary(UMaterial::StaticClass(), true, GIsEditor);
	//	ObjectLibrary->AddToRoot();
	//}
	////IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	////FString ContentDir = FPaths::GameContentDir() + "/StarterContent/Materials/";
	//ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game/StarterContent/Materials/"));
	//// example: Material'/Game/StarterContent/Materials/M_Ground_Grass.M_Ground_Grass'
	//ObjectLibrary->LoadAssetsFromAssetData();
	//ObjectLibrary->GetAssetDataList(AssetDatas);	
	//// todo: filepath to assets

	//TArray<FStringAssetReference> ItemsToLoad;
	//for (auto& Asset : AssetDatas)
	//{
	//	ItemsToLoad.Add(Asset.ToStringReference());
	//}
	//RequestLoad(ItemsToLoad);
}

// Code to load a singe element by key - decided to load all at the start
//FStringAssetReference UCustomizationService::LoadMaterial(const std::string& key)
//{
//	for (int32 i = 0; i < AssetDatas.Num(); ++i)
//	{
//		FAssetData& AssetData = AssetDatas[i];
//
//		const FString* FoundTypeNameString = AssetData.TagsAndValues.Find(GET_MEMBER_NAME_CHECKED(key, TypeName));
//
//		if (FoundTypeNameString && FoundTypeNameString->Contains(UTF8_TO_TCHAR(key.c_str())))
//		{
//			return AssetData.ToStringReference();
//;
//		}
//	}
//}
//
//void UCustomizationService::RequestLoad(TArray<FStringAssetReference>& ItemsToLoad)
//{
//	if (LoadRequestedMaterials.Num())
//	{
//		UE_LOG(UtyProcessing, Error, TEXT("Load material request still in process, ignoring new request."));
//		return;
//	}
//	
//	LoadRequestedMaterials.Append(ItemsToLoad);
//	Streamable.RequestAsyncLoad(LoadRequestedMaterials, FStreamableDelegate::CreateUObject(this, &UCustomizationService::OnLoadComplete));
//}
//void UCustomizationService::OnLoadComplete()
//{
//	for (int32 i = 0; i < LoadRequestedMaterials.Num(); ++i)
//	{
//		UMaterial* Material = Cast<UMaterial>(LoadRequestedMaterials[i].ResolveObject());
//		if (Material)
//		{
//			_sharedMaterials.Add(Material->GetName(), Material);
//		}
//	}
//}

UMaterialInstanceDynamic* UCustomizationService::GetSharedMaterial(const std::string& key)
{
	std::string prefix("Materials/info_icons");
	if (key.compare(0, prefix.size(), prefix) == 0)
	{
		return MaterialInstanceIcons;
	}

	prefix = "Materials/M_Shaded_Color";
	if (MaterialInstance && (key.compare(0, prefix.size(), prefix) == 0))
	{
		return MaterialInstance;
	}
	else
	{
		// default to texture atlas
		UE_LOG(UtyProcessing, Warning, TEXT("Default material used for unknown request"));
		return MaterialInstance;
	}

	/*if (_sharedMaterials.Num())
	{
		UMaterial* FoundMaterial = *(_sharedMaterials.Find(FString(UTF8_TO_TCHAR(key.c_str()))));
		if (FoundMaterial)
		{
			return FoundMaterial;
		}
		else
		{
			UE_LOG(UtyProcessing, Error, TEXT("Cannot find %s"), UTF8_TO_TCHAR(key.c_str()));
			return nullptr;
		}
	}
	else
	{
		UE_LOG(UtyProcessing, Error, TEXT("Cannot find %s"), UTF8_TO_TCHAR(key.c_str()));
		return nullptr;
	}*/
}
