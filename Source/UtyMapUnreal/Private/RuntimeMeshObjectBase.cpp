// Copyright 2016 Chris Conway (Koderz). All Rights Reserved.

#include "UtyMapUnreal.h"
#include "RuntimeMeshObjectBase.h"

ARuntimeMeshObjectBase::ARuntimeMeshObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RuntimeMesh = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("Runtime Mesh"));
	RootComponent = RuntimeMesh;
}

void ARuntimeMeshObjectBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARuntimeMeshObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
