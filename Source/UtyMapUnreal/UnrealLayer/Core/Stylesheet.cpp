#include "UtyMapUnreal.h"
#include "Stylesheet.h"


UStylesheet::UStylesheet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

const std::string& UStylesheet::GetPath() const
{
	return Filename;
}

void UStylesheet::SetFilename(const std::string& value)
{
	Filename = value;
}

UStylesheet::UStylesheet(const std::string& path)
{
	SetFilename(path);
}
