#pragma once

#include <string>

class FStaticStrings
{
	// errors
public:
	static std::string CannotRegisteActionIfSetupIsComplete;
	static std::string SetupIsCalledMoreThanOnce;
	static std::string CannotFindSrtmData;

	// information
	static std::string LoadElevationFrom;
	static std::string NoPresistentElementSourceFound;
	static std::string CannotAddDataToInMemoryStore;
};