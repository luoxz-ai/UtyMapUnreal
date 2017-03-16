#pragma once

#include "Tile.h"
#include <string>
#include <mutex>
#include "NoExportTypes.h"
#include "IMapDataProvider.generated.h"

// Provides basic functionality for map data downloading and file storing.
UCLASS()
class UMapDataProvider : public UObject
{
	GENERATED_BODY()

private:
	std::mutex _lockObj;

protected:
	std::string* Get(ATile* tile, const std::string& uri, const std::string& filePath);

	// Writes bytes to file with preprocessing if necessary.
	void WriteBytes(std::ifstream* stream, unsigned char bytes[]);
};
			