// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "ExportLib.hpp"
#include "UtyMapLoader.generated.h"

UCLASS()
class UTYMAPUNREAL_API AUtyMapLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUtyMapLoader();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends or when destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override
		;
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	/* The holy grain of all meshes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utymap")
		URuntimeMeshComponent* RuntimeMesh = nullptr;

private:
	UStaticMeshComponent* MeshComponent;
	
	static bool hasNewData;
	int32 CurrentMeshSectionIndex;

	static TArray<FVector> UnrealVerticies;
	static TArray<int32> UnrealTriangles;
	static TArray<FColor> UnrealColors;
	static TArray<FVector2D> UnrealUV0s;
	static TArray<FVector2D> UnrealUV1s;
	
	static void OnError(const char* msg);
	static void OnMeshBuilt(const char* name,                       // name 
		const double* vertices, int vertexSize, // vertices (x, y, elevation)
		const int* triangles, int triSize,      // triangle indices
		const int* colors, int colorSize,       // rgba colors
		const double* uvs, int uvSize,          // absolute texture uvs
		const int* uvMap, int uvMapSize);       // map with info about used atlas and texture region

	static void OnElementLoaded(uint64 id,                       // element id
		const char** tags, int tagsSize,        // tags
		const double* vertices, int vertexSize, // vertices (x, y, elevation)
		const char** style, int styleSize);     // mapcss styles (key, value)

	void loadQuadKeys(int levelOfDetails, int startX, int endX, int startY, int endY) const;
};
