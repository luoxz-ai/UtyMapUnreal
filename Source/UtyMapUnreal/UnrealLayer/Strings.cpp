#include "UtyMapUnreal.h"
#include "Strings.h"

std::string FStaticStrings::CannotRegisteActionIfSetupIsComplete = std::string("Cannot register action if setup is complete.");
std::string FStaticStrings::SetupIsCalledMoreThanOnce = std::string("Cannot perform setup more than once.");
std::string FStaticStrings::CannotFindSrtmData = std::string("SRTM data cell not found: {0}");
std::string FStaticStrings::LoadElevationFrom = std::string("Load elevation from {0}..");
std::string FStaticStrings::NoPresistentElementSourceFound = std::string("No offline map data found for {0}, will query default server: {1}");
std::string FStaticStrings::CannotAddDataToInMemoryStore = std::string("Cannot add data to in memory store: {0}");
