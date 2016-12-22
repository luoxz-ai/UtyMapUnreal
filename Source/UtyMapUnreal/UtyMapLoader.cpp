// Fill out your copyright notice in the Description page of Project Settings.

#include "UtyMapUnreal.h"
#include "UtyMapLoader.h"


// test defines
#define TEST_ASSETS_PATH "C:/workspace/projects/utymap/core/test/test_assets/"

#define TEST_EXTERNAL_ASSETS_PATH TEST_ASSETS_PATH "../../../unity/demo/Assets/StreamingAssets/"

#define TEST_MAPCSS_PATH TEST_ASSETS_PATH "mapcss/"
#define TEST_MAPCSS_DEFAULT TEST_EXTERNAL_ASSETS_PATH "MapCss/default/default.mapcss"

#define TEST_JSON_FILE TEST_EXTERNAL_ASSETS_PATH "Osm/moscow.osm.json"
#define TEST_PBF_FILE TEST_EXTERNAL_ASSETS_PATH "Osm/berlin.osm.pbf"
#define TEST_XML_FILE TEST_EXTERNAL_ASSETS_PATH "Osm/berlin.osm.xml"

#define TEST_SHAPE_POINT_FILE TEST_ASSETS_PATH "shape/artificial/point"
#define TEST_SHAPE_LINE_FILE TEST_ASSETS_PATH "shape/artificial/line"
#define TEST_SHAPE_POLY_FILE TEST_ASSETS_PATH "shape/artificial/poly"
#define TEST_SHAPE_MULTIPOLY_FILE TEST_ASSETS_PATH "shape/artificial/multipoly"

#define TEST_SHAPE_NE_110M TEST_EXTERNAL_ASSETS_PATH "NaturalEarth/"
#define TEST_SHAPE_NE_110M_LAND TEST_SHAPE_NE_110M "ne_110m_land"
#define TEST_SHAPE_NE_110M_RIVERS TEST_SHAPE_NE_110M "ne_110m_rivers_lake_centerlines"
#define TEST_SHAPE_NE_110M_LAKES TEST_SHAPE_NE_110M "ne_110m_lakes"
#define TEST_SHAPE_NE_110M_ADMIN TEST_SHAPE_NE_110M "ne_110m_admin_0_scale_rank"
#define TEST_SHAPE_NE_110M_BORDERS TEST_SHAPE_NE_110M "ne_110m_admin_0_boundary_lines_land"
#define TEST_SHAPE_NE_110M_POPULATED_PLACES TEST_SHAPE_NE_110M "ne_110m_populated_places_simple"

// Use global variable as the export library callbacks don't like class functions (use std::bind? )
bool AUtyMapLoader::hasNewData = false;
TArray<FVector> AUtyMapLoader::UnrealVerticies;
TArray<int32> AUtyMapLoader::UnrealTriangles;
TArray<FColor> AUtyMapLoader::UnrealColors;
TArray<FVector2D> AUtyMapLoader::UnrealUV0s;
TArray<FVector2D> AUtyMapLoader::UnrealUV1s;

// Sets default values
AUtyMapLoader::AUtyMapLoader()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh container for the whole tile, meaning all meshes are part of this single mesh as seperate sections.
	RuntimeMesh = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("Runtime Mesh"));
	RootComponent = RuntimeMesh;

	if (RuntimeMesh)
	{
		RootComponent = RuntimeMesh;
	}
}

// Called when the game starts or when spawned
void AUtyMapLoader::BeginPlay()
{
	Super::BeginPlay();

	CurrentMeshSectionIndex = 0;

	const char* InMemoryStoreKey = "InMemory";
	configure(TEST_ASSETS_PATH, OnError);
	registerInMemoryStore(InMemoryStoreKey);
	addToStoreInRange(InMemoryStoreKey, TEST_MAPCSS_DEFAULT, TEST_SHAPE_NE_110M_LAND, 1, 1, AUtyMapLoader::OnError);

	addToStoreInRange(InMemoryStoreKey, TEST_MAPCSS_DEFAULT, TEST_SHAPE_NE_110M_RIVERS, 1, 1, AUtyMapLoader::OnError);
	addToStoreInRange(InMemoryStoreKey, TEST_MAPCSS_DEFAULT, TEST_SHAPE_NE_110M_LAKES, 1, 1, AUtyMapLoader::OnError);

	//This data increases execution time. Also causes some issues.
	addToStoreInRange(InMemoryStoreKey, TEST_MAPCSS_DEFAULT, TEST_SHAPE_NE_110M_ADMIN, 1, 1, AUtyMapLoader::OnError);
	addToStoreInRange(InMemoryStoreKey, TEST_MAPCSS_DEFAULT, TEST_SHAPE_NE_110M_BORDERS, 1, 1, AUtyMapLoader::OnError);

	addToStoreInRange(InMemoryStoreKey, TEST_MAPCSS_DEFAULT, TEST_SHAPE_NE_110M_POPULATED_PLACES, 1, 1, AUtyMapLoader::OnError);

	loadQuadKeys(1, 0, 1, 0, 1);

}

void AUtyMapLoader::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	cleanup();
	std::remove((std::string(TEST_ASSETS_PATH) + "string.idx").c_str());
	std::remove((std::string(TEST_ASSETS_PATH) + "string.dat").c_str());
}

// Called every frame
void AUtyMapLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<FVector> UnrealNormals;
	TArray<FRuntimeMeshTangent> UnrealTangents;
	if (hasNewData)
	{
		RuntimeMesh->CreateMeshSection(CurrentMeshSectionIndex, UnrealVerticies, UnrealTriangles, UnrealNormals, UnrealUV0s, UnrealColors, UnrealTangents, true, EUpdateFrequency::Infrequent, ESectionUpdateFlags::CalculateNormalTangent);
		hasNewData = false;
		CurrentMeshSectionIndex++;
	}
}

void AUtyMapLoader::OnError(const char* msg)
{
	UE_LOG(LogTemp, Error, TEXT("Utymap error: %s"), UTF8_TO_TCHAR(msg));
	return;
}

void AUtyMapLoader::OnMeshBuilt(const char* name, /* name */ const double* vertices, int vertexSize, /* vertices (x, y, elevation) */ const int* triangles, int triSize, /* triangle indices */ const int* colors, int colorSize, /* rgba colors */ const double* uvs, int uvSize, /* absolute texture uvs */ const int* uvMap, int uvMapSize)
{
	if (hasNewData)
	{
		UE_LOG(LogTemp, Error, TEXT("mesh not processed yet"));
	}
	UnrealVerticies.Empty();
	UnrealTriangles.Empty();
	UnrealColors.Empty();
	UnrealUV0s.Empty();

	// Unreal uses 1cm as its Unreal Units standard. So all vectors need to be scaled. commented section cannot be run on static functions
	/*float WorldToMeters = GetWorld()->GetWorldSettings()->WorldToMeters;*/
	float WorldToMetersScale = 100.0;

	if (vertices)
	{
		UnrealVerticies.Reserve(vertexSize);
		for (int vertex = 0; vertex < vertexSize; vertex = vertex + 3)
		{
			UnrealVerticies.Add(FVector(
				vertices[vertex] * WorldToMetersScale,
				vertices[vertex + 1] * WorldToMetersScale,
				vertices[vertex + 2] * WorldToMetersScale)
			);
		}
	}

	if (triangles)
	{
		UnrealTriangles.Reserve(triSize);
		for (int triangle = 0; triangle < triSize; triangle++)
		{
			UnrealTriangles.Add(triangles[triangle]);
		}
	}

	if (colors)
	{
		UnrealColors.Reserve(colorSize);
		for (int color = 0; color < colorSize; color++)
		{
			UnrealColors.Add(FColor(colors[color]));
		}
	}

	if (uvs)
	{
		UnrealUV0s.Reserve(uvSize);
		for (int uv = 0; uv < uvSize; uv = uv + 2)
		{
			UnrealUV0s.Add(FVector2D(
				uvs[uv],
				uvs[uv + 1]
			));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Utymap mesh built: %s"), UTF8_TO_TCHAR(name));
	hasNewData = true;
}

void AUtyMapLoader::OnElementLoaded(uint64 id, /* element id */ const char** tags, int tagsSize, /* tags */ const double* vertices, int vertexSize, /* vertices (x, y, elevation) */ const char** style, int styleSize)
{
	UE_LOG(LogTemp, Warning, TEXT("Utymap element id %i loaded"), UTF8_TO_TCHAR(std::to_string(id).c_str()));
}

void AUtyMapLoader::loadQuadKeys(int levelOfDetails, int startX, int endX, int startY, int endY) const
{
	for (int i = startX; i <= endX; ++i) {
		for (int j = startY; j <= endY; ++j) {
			loadQuadKey(TEST_MAPCSS_DEFAULT, i, j, levelOfDetails, 0,
				&AUtyMapLoader::OnMeshBuilt,
				&AUtyMapLoader::OnElementLoaded,
				&AUtyMapLoader::OnError
			);
		}
	}
}
