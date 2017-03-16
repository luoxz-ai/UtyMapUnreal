#pragma once

#include "ElevationDataType.generated.h"

// Specifies which elevation data will be used.
UENUM(BlueprintType)
enum class EElevationDataType : uint8
{
	Flat 	UMETA(DisplayName = "Flat"),
	Srtm 	UMETA(DisplayName = "SRTM"),
	Grid	UMETA(DisplayName = "Grid")
};		