#include "UtyMapUnreal.h"
#include "MapTileAdapter.h"
#include "ColorUtils.h"
#include "Mesh.h"
#include "Element.h"
#include "Range.h"
#include "UnrealString.h"
#include "Regex.h"

void UMapTileAdapter::Init(ATile* Tile, ADemoModelBuilder* ModelBuilder)
{
	_tile = Tile;
	if (_modelBuilder != ModelBuilder && ModelBuilder->IsValidLowLevel())
	{
		_modelBuilder = ModelBuilder;
		UE_LOG(UtyProcessing, Warning, TEXT("connected new modelbuilder for tile %s"), UTF8_TO_TCHAR(Tile->ToString().c_str()));

		MeshBuilt.AddDynamic(ModelBuilder, &ADemoModelBuilder::BuildMesh);
		ElementBuilt.AddDynamic(ModelBuilder, &ADemoModelBuilder::BuildElement);
	}
}

void UMapTileAdapter::Shutdown()
{
	MeshBuilt.RemoveAll(_modelBuilder);
	ElementBuilt.RemoveAll(_modelBuilder);
}

int UMapTileAdapter::BroadCastNew()
{
	_lockObj.lock();

	for (auto element : NewlyBuiltElements)
	{
		ElementBuilt.Broadcast(_tile, element);
		OutStandingJobs--;
	}
	NewlyBuiltElements.Reset();

	for (auto Mesh : NewlyBuiltMeshes)
	{
		MeshBuilt.Broadcast(_tile, Mesh);
		OutStandingJobs--;
	}
	NewlyBuiltMeshes.Reset();

	_lockObj.unlock();
	return OutStandingJobs;
}

void UMapTileAdapter::AdaptMesh(const std::string& name, const double vertices[], const int vertexCount, const int triangles[], int triangleCount, const int colors[], int colorCount, const double uvs[], int uvCount, const int uvMap[], int uvMapCount)
{
	if (_tile == nullptr)
	{
		UE_LOG(UtyProcessing, Error, TEXT("Received mesh for a tile that is not valid anymore"));
		return;
	}

	if (!MeshBuilt.IsBound())
	{
		UE_LOG(UtyProcessing, Error, TEXT("No mesh builders registerd. dropping conversion from core library"));
		return;
	}

	TArray<FVector> VerticesUnreal;
	TArray<int32> TrianglesUnreal;
	TArray<FColor> ColorsUnreal;

	TArray<FVector2D> UVsUnreal;
	TArray<FVector2D> UVs2Unreal;
	TArray<FVector2D> UVs3Unreal;

	TArray<int32> UVMapConverted;
	TArray<double> UvsConverted;

	// NOTE process terrain differently to emulate flat shading effect by avoiding 
	// triangles to share the same vertex. Remove "if" branch if you don't need it
	std::string prefix("terrain");
	if (!name.compare(0, prefix.size(), prefix))
	{
		VerticesUnreal.AddUninitialized(triangleCount);
		ColorsUnreal.AddUninitialized(triangleCount);
		TrianglesUnreal.AddUninitialized(triangleCount);
		UVsUnreal.AddUninitialized(triangleCount);
		UVs2Unreal.AddUninitialized(triangleCount);
		UVs3Unreal.AddUninitialized(triangleCount);
		
		UvsConverted.AddUninitialized(uvCount);
		UVMapConverted.AddUninitialized(uvMapCount);

		for (int i = 0; i < uvCount; i++)
		{
			UvsConverted[i] = uvs[i];
		}
		for (int i = 0; i < uvMapCount; i++)
		{
			UVMapConverted[i] = uvMap[i];
		}

		auto TextureMapper = CreateTextureAtlasMapper(UVsUnreal, UVs2Unreal, UVs3Unreal, UvsConverted, UVMapConverted);
						
		for (int i = 0; i < triangleCount; i++)
		{
			int vertIndex = triangles[i] * 3;
			FGeoCoordinate tempVar(vertices[vertIndex + 1], vertices[vertIndex]);
			VerticesUnreal[i] = *(_tile->GetProjection()->Project(tempVar, vertices[vertIndex + 2]));

			ColorsUnreal[i] = ColorUtils::FromInt(colors[triangles[i]]);
			TextureMapper->SetUvs(i, triangles[i] * 2);
			TrianglesUnreal[i] = i;
		}

		// invert indices of each triangle to flip normals 
		// TODO: seems to cause culling issues. or is it the bounding box?
		for (int i = 0; i < triangleCount; i+=3)
		{
			auto Swap = TrianglesUnreal[i + 2];
			TrianglesUnreal[i + 2] = TrianglesUnreal[i];
			TrianglesUnreal[i] = Swap;
		}
	}
	else
	{
		long long id = 0;
		if (!ShouldLoad(name, id))
		{
			return;
		}

		VerticesUnreal.AddUninitialized(vertexCount / 3);
		for (int i = 0; i < vertexCount; i += 3)
		{
			FGeoCoordinate tempVar2(vertices[i + 1], vertices[i]);
			VerticesUnreal[i / 3] = *(_tile->GetProjection()->Project(tempVar2, vertices[i + 2]));
		}
		
		// invert indices of each triangle to flip normals
		TrianglesUnreal.AddUninitialized(triangleCount);
		for (int i = 0; i < triangleCount; i += 3 )
		{
			TrianglesUnreal[i] = triangles[i+2];
			TrianglesUnreal[i+1] = triangles[i+1];
			TrianglesUnreal[i+2] = triangles[i];
		}

		ColorsUnreal.AddUninitialized(colorCount);
		for (int i = 0; i < colorCount; ++i)
		{
			ColorsUnreal[i] = ColorUtils::FromInt(colors[i]);
		}

		if (uvCount > 0)
		{
			UVsUnreal.AddUninitialized(uvCount / 2);
			UVs2Unreal.AddUninitialized(uvCount / 2);
			UVs3Unreal.AddUninitialized(uvCount / 2);

			// TODO: could also pass raw array to texture mapper instead to avoid copy
			UvsConverted.AddUninitialized(uvCount);
			UVMapConverted.AddUninitialized(uvMapCount);

			for (int i = 0; i < uvCount; i += 2)
			{
				UvsConverted[i] = uvs[i];
				UvsConverted[i + 1] = 1 - uvs[i + 1];
			}
			for (int i = 0; i < uvMapCount; i++)
			{
				UVMapConverted[i] = uvMap[i];
			}

			auto TextureMapper = CreateTextureAtlasMapper(UVsUnreal, UVs2Unreal, UVs3Unreal, UvsConverted, UVMapConverted);
			for (int i = 0; i < uvCount; i += 2)
			{
				// unreal has other uv coordinate origins than unity. top left (ue4), bottom left (unity)
				UVsUnreal[i / 2] = FVector2D(static_cast<float>(uvs[i]), static_cast<float>(uvs[i + 1]));
				TextureMapper->SetUvs(i / 2, i);
			}
		}
		else
		{
			UVsUnreal.AddUninitialized(VerticesUnreal.Num());
			UVs2Unreal.AddUninitialized(VerticesUnreal.Num());
			UVs3Unreal.AddUninitialized(VerticesUnreal.Num());
		}
		
		_tile->Register(id);

		if (VerticesUnreal.Num() >= 65000)
		{
			UE_LOG(UtyProcessing, Warning, TEXT("Mesh '%s' has more vertices than allowed: %s. It should be split but this is missing functionality in UtyMap.Unity."), *FString(name.c_str()), *FString::FromInt(VerticesUnreal.Num()));
		}
	}
	BuildMesh(name, VerticesUnreal, TrianglesUnreal, ColorsUnreal, UVsUnreal, UVs2Unreal, UVs3Unreal);

}

void UMapTileAdapter::BuildMesh(const std::string &name, TArray<FVector>& VerticesUnreal, TArray<int32>& TrianglesUnreal, TArray<FColor>& ColorsUnreal, TArray<FVector2D>& UVsUnreal, TArray<FVector2D>& UVs2Unreal, TArray<FVector2D>& UVs3Unreal)
{
	// Create a Mesh
	FMesh Mesh;
	Mesh.Name = FString(name.c_str());
	// TODO: make dependent on mesh type when texture atlas is an optional feature
	Mesh.TextureIndex = 0;
	Mesh.Vertices = VerticesUnreal;
	Mesh.Triangles = TrianglesUnreal;
	Mesh.Colors = ColorsUnreal;
	Mesh.Uvs = UVsUnreal;
	Mesh.Uvs2 = UVs2Unreal;
	Mesh.Uvs3 = UVs3Unreal;

	//UE_LOG(UtyProcessing, Warning, TEXT("Mesh adapted"));
	_lockObj.lock();
	NewlyBuiltMeshes.Add(Mesh);
	OutStandingJobs++;
	_lockObj.unlock();
}

void UMapTileAdapter::AdaptElement(long long id, const char** tags, int tagCount, const double vertices[], int vertexCount, const char** styles, int styleCount)
{
	if (_tile == nullptr)
	{
		// TODO: cancelation token should solve this
		UE_LOG(UtyProcessing, Error, TEXT("Received mesh for a tile that is not valid anymore"));
		return;
	}

	std::vector<FGeoCoordinate> geometry(vertexCount / 3);
	std::vector<double> heights( vertexCount / 3 );
	for (int i = 0; i < vertexCount / 3; i += 3)
	{
		geometry[i / 3] = FGeoCoordinate(vertices[i + 1], vertices[i]);
		heights[ i / 3 ] = vertices[ i + 2 ];
	}

	FElement element;
	element.Id = id;
	element.Geometry = geometry;
	element.Heights = heights;
	element.Tags = ReadDict(tags, tagCount);
	element.Styles = ReadDict(styles, styleCount);

	//UE_LOG(UtyProcessing, Warning, TEXT("Element adapted"));
	_lockObj.lock();
	NewlyBuiltElements.Add(element);
	OutStandingJobs++;
	_lockObj.unlock();
}

std::unordered_map<std::string, std::string> UMapTileAdapter::ReadDict(const char** data, int dataSize)
{
	auto map = std::unordered_map<std::string, std::string>(dataSize / 2);
	for (int i = 0; i < dataSize ; i += 2)
	{
		map.insert(std::make_pair(data[ i ], data[ i + 1 ]));
	}
	return map;
}

bool UMapTileAdapter::ShouldLoad(const std::string& name, long long& id)
{
	const FRegexPattern myPattern(TEXT("^(building):([0-9]*)"));
	FRegexMatcher myMatcher(myPattern, UTF8_TO_TCHAR(name.c_str()));

	if (myMatcher.FindNext())
	{
		id = std::atoll( TCHAR_TO_UTF8(*(myMatcher.GetCaptureGroup(2))));
	}
	else
	{
		id = 0;
		return true;
	}
	return !_tile->Has(id);
}

UMapTileAdapter::FTextureAtlasMapper* UMapTileAdapter::CreateTextureAtlasMapper(TArray<FVector2D>& UnrealUVs, TArray<FVector2D>& UnrealUVs2, TArray<FVector2D>& UnrealUVs3, TArray<double>& UvsIn, TArray<int32>& UuvMapIn)
{
	constexpr int infoEntrySize = 8;
	auto count = UuvMapIn.Num();
	TArray<FTextureAtlasInfo> infos; infos.Reserve(count / infoEntrySize);
	for (int i = 0; i < count;)
	{
		auto info = FTextureAtlasInfo();
		info.UvIndexRange = new TLodRange<int>(!infos.Num() ? 0 : infos.Last().UvIndexRange->Maximum, UuvMapIn[ i++ ]);
		info.TextureIndex = UuvMapIn[ i++ ];

		int atlasWidth = UuvMapIn[ i++ ];
		int atlasHeight = UuvMapIn[ i++ ];
		float x = UuvMapIn[ i++ ];
		float y = UuvMapIn[ i++ ];
		float width = UuvMapIn[ i++ ];
		float height = UuvMapIn[ i++ ];

		bool isEmpty = atlasWidth == 0 || atlasHeight == 0;
		info.TextureSize = new FVector2D(isEmpty ? 0 : width / atlasWidth, isEmpty ? 0 : height / atlasHeight);
		info.TextureOffset = new FVector2D(isEmpty ? 0 : x / atlasWidth, isEmpty ? 0 : y / atlasHeight);

		infos.Add(info);
	}

	return new FTextureAtlasMapper(UnrealUVs, UnrealUVs2, UnrealUVs3, UvsIn, infos);
}

				
UMapTileAdapter::FTextureAtlasMapper::FTextureAtlasMapper(TArray<FVector2D>& Unreal1, TArray<FVector2D>& Unreal2, TArray<FVector2D>& Unreal3, TArray<double>& UtyUVs, TArray<FTextureAtlasInfo> infos) : UnrealUVs(Unreal1), UnrealUVs2(Unreal2), UnrealUVs3(Unreal3), UVs(UtyUVs), Infos(infos)
{
}

void UMapTileAdapter::FTextureAtlasMapper::SetUvs(int resultIndex, int origIindex)
{
	int begin = 0;
	int end = Infos.Num();

	while (begin < end)
	{
		int middle = begin + (end - begin) / 2;
		FTextureAtlasInfo info = Infos[ middle ];
		if (info.UvIndexRange->Contains(origIindex))
		{
			UnrealUVs[ resultIndex ] = FVector2D(static_cast<float>(UVs[ origIindex ]), static_cast<float>(UVs[ origIindex + 1 ]));
			UnrealUVs2[ resultIndex ] = FVector2D(*info.TextureSize);
			UnrealUVs3[resultIndex] = FVector2D(info.TextureOffset->X, info.TextureOffset->Y);
			return;
		}
		if (info.UvIndexRange->Minimum > origIindex)
		{
			end = middle;
		}
		else
		{
			begin = middle + 1;
		}
	}
}