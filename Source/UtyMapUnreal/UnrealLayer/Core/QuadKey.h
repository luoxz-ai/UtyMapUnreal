#pragma once

#include <string>
#include "QuadKey.generated.h"

// Represents quadkey
USTRUCT()
struct FQuadKey
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int TileX;
	UPROPERTY(BlueprintReadOnly)
	int TileY;
	UPROPERTY(BlueprintReadOnly)
	int LevelOfDetail;

	// Creates QuadKey
	FQuadKey(int tileX, int tileY, int levelOfDetail);
				
	// Todo: not appropriate to implement this, just to please the compiler
	FQuadKey() : TileX(0), TileY(0), LevelOfDetail(0) {};

	FQuadKey& operator=(const FQuadKey& other)
	{
		TileX = other.TileX;
		TileY = other.TileY;
		LevelOfDetail = other.LevelOfDetail;
		return *this;
	}

	bool Equals(FQuadKey other);
	virtual bool Equals(void* obj);
	virtual int GetHashCode();
	virtual std::string ToString();
};

inline bool operator==(const FQuadKey& lhs, const FQuadKey& rhs)
{
	return ((lhs.TileX == rhs.TileX) && (lhs.TileY == rhs.TileY) && (lhs.LevelOfDetail == rhs.LevelOfDetail));
}