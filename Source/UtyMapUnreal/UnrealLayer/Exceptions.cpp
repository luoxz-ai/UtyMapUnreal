#include "UtyMapUnreal.h"
#include "Exceptions.h"

FMapDataException::FMapDataException(const std::string& message)
{
	UE_LOG(UtyInit, Error, TEXT("%S"), UTF8_TO_TCHAR(message.c_str()));
}
FMapDataException::FMapDataException(const std::string& formatStr, const std::string& message)
{
	UE_LOG(UtyInit, Error, TEXT("%s"), UTF8_TO_TCHAR(formatStr.c_str()), UTF8_TO_TCHAR(message.c_str()));
}