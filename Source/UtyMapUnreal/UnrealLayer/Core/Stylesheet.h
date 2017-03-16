#pragma once

#include <string>
#include "Stylesheet.generated.h"

// Represents stylesheet
UCLASS()
class UStylesheet : public UObject
{
	GENERATED_BODY()

private:
	std::string Filename;

public:
	UStylesheet(const FObjectInitializer& ObjectInitializer);
	const std::string&  GetPath() const;
	void SetFilename(const std::string& value);

	UStylesheet(const std::string& path);
};