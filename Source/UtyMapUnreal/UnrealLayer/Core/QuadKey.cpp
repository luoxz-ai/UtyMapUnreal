#include "UtyMapUnreal.h"
#include "QuadKey.h"

FQuadKey::FQuadKey(int tileX, int tileY, int levelOfDetail) : TileX(tileX), TileY(tileY), LevelOfDetail(levelOfDetail)
{
}

bool FQuadKey::Equals(FQuadKey other)
{
	return TileX == other.TileX && TileY == other.TileY && LevelOfDetail == other.LevelOfDetail;
}

bool FQuadKey::Equals(void* obj)
{
	//if (!(dynamic_cast<FQuadKey>(obj) != nullptr))
	if (obj == nullptr)
	{
		return false;
	}
	return Equals(*(static_cast<FQuadKey*>(obj)));
}

int FQuadKey::GetHashCode()
{
	int hash = 17;
	// Todo: implement hash function
	//hash = hash * 29 + TileX.GetHashCode();
	//hash = hash * 29 + TileY.GetHashCode();
	//hash = hash * 29 + LevelOfDetail.GetHashCode();
	return hash;
}

std::string FQuadKey::ToString()
{
	std::string code;
	code.reserve(static_cast<std::size_t>(LevelOfDetail));
	for (int i = LevelOfDetail; i > 0; --i) {
		char digit = '0';
		int mask = 1 << (i - 1);
		if ((TileX & mask) != 0)
			digit++;
		if ((TileY & mask) != 0) {
			digit++;
			digit++;
		}
		code += digit;
	}
	return code;
}
