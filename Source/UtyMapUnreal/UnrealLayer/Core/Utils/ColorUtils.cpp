#include "UtyMapUnreal.h"
#include "ColorUtils.h"

FColor ColorUtils::FromInt(int rgba)
{
	auto r = static_cast<unsigned char>((rgba >> 24) & 0xff);
	auto g = static_cast<unsigned char>((rgba >> 16) & 0xff);
	auto b = static_cast<unsigned char>((rgba >> 8) & 0xff);
	auto a = static_cast<unsigned char>(rgba & 0xff);

	return FColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}
