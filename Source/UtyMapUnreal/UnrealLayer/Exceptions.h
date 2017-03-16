#pragma once

#include <string>
#include <stdexcept>

/**
 *Map data exception representation
 */
class FMapDataException
{
	FMapDataException(const std::string& message);
	FMapDataException(const std::string& formatStr, const std::string& message);
};
