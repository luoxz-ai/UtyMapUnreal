#pragma once

#include "NoExportTypes.h"
#include "Tile.h"
#include "Range.h"
#include "Mesh.h"
#include "GeoCoordinate.h"
#include <vector>
#include <mutex>
#include "Union.h"
#include "Range.h"
#include "DemoModelBuilder.h"
#include "MapTileAdapter.generated.h"


// send newly built mesh back to MapDataLoader
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMeshBuilt, ATile*, Tile, FMesh&, Mesh);

// send newly built mesh back to MapDataLoader
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FElementBuilt, ATile*, Tile, FElement&, Element);


/// Adapts map tile data received from utymap API to the type used by the app.
///
UCLASS()
class UMapTileAdapter : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	ADemoModelBuilder* _modelBuilder;
	UPROPERTY()
	ATile* _tile;
	//TUnion<FElement*, FMesh*>*  _data; // either do oberserver pattern here or save data and signal the update

public:
	UPROPERTY()
	FMeshBuilt MeshBuilt;
	UPROPERTY()
	FElementBuilt ElementBuilt;

	UPROPERTY()
	TArray<FElement> NewlyBuiltElements;
	UPROPERTY()
	TArray<FMesh> NewlyBuiltMeshes;

	// Adapts mesh data received from utymap.
	void AdaptMesh(const std::string& name, const double vertices[], const int vertexCount, const int triangles[], int triangleCount, const int colors[], int colorCount, const double uvs[], int uvCount, const int uvMap[], int uvMapCount);

	void BuildMesh(const std::string &name, TArray<FVector>& VerticesUnreal, TArray<int32>& TrianglesUnreal, TArray<FColor>& ColorsUnreal, TArray<FVector2D>& UVsUnreal, TArray<FVector2D>& UVs2Unreal, TArray<FVector2D>& UVs3Unreal);

	// Adapts element data received from utymap.
	void AdaptElement(long long id, const char** tags, int tagCount, const double vertices[], int vertexCount, const char** styles, int styleCount);

	void Init(ATile* Tile, ADemoModelBuilder* ModelBuilder);

	void Shutdown();

	int BroadCastNew();

private:
	std::mutex _lockObj;
	int OutStandingJobs = 0;
	static std::unordered_map<std::string, std::string> ReadDict(const char** data, int dataSize);

	bool ShouldLoad(const std::string& name, long long& id);

	class FTextureAtlasInfo
	{
	public:
		int TextureIndex = 0;
		TLodRange<int>* UvIndexRange;
		FVector2D* TextureSize;
		FVector2D* TextureOffset;
	};

	class FTextureAtlasMapper
	{
	private:
		TArray<FVector2D>& UnrealUVs;
		TArray<FVector2D>& UnrealUVs2;
		TArray<FVector2D>& UnrealUVs3;
		TArray<double>& UVs;
		TArray<FTextureAtlasInfo> Infos;

	public:
		FTextureAtlasMapper(TArray<FVector2D>& Unreal1, TArray<FVector2D>& Unreal2, TArray<FVector2D>& Unreal3, TArray<double>& UtyUVs, TArray<FTextureAtlasInfo> infos);

		void SetUvs(int resultIndex, int origIindex);
	};

	static FTextureAtlasMapper* CreateTextureAtlasMapper(TArray<FVector2D> &unityUvs, TArray<FVector2D> &unityUvs2, TArray<FVector2D> &unityUvs3, TArray<double> &uvs, TArray<int32> &uvMap);
};