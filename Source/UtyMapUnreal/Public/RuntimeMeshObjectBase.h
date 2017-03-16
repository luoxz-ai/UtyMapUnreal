// Copyright 2016 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "RuntimeMeshObjectBase.generated.h"

UCLASS()
class UTYMAPUNREAL_API ARuntimeMeshObjectBase : public AActor
{
	GENERATED_BODY()

public:
	ARuntimeMeshObjectBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
		URuntimeMeshComponent* RuntimeMesh;
};