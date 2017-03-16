#pragma once

#include "NoExportTypes.h"
#include <string>
#include "Mesh.generated.h"

/**
 *Represents mesh with all geometry needed.
 */
USTRUCT(BlueprintType, Blueprintable)
struct FMesh
{
	GENERATED_USTRUCT_BODY()

public:
	FString Name;
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FColor> Colors;

	int32 TextureIndex;
	TArray<FVector2D> Uvs;
	TArray<FVector2D> Uvs2;
	TArray<FVector2D> Uvs3;
/*
	
	FMesh(const FString& name, int32 TextureIndex, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FColor>& Colors, TArray<FVector2D>& Uvs, TArray<FVector2D>& Uvs2, TArray<FVector2D>& Uvs3);*/
};
