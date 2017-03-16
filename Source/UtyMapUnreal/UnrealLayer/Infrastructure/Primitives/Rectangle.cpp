#include "UtyMapUnreal.h"
#include "Rectangle.h"


FRectangle::FRectangle(float x, float y, float width, float height) : _xmin(x), _ymin(y), _xmax(x + width), _ymax(y + height)
{
}

const float& FRectangle::GetLeft() const
{
	return _xmin;
}

const float& FRectangle::GetRight() const
{
	return _xmax;
}

const float& FRectangle::GetBottom() const
{
	return _ymin;
}

const float& FRectangle::GetTop() const
{
	return _ymax;
}

FVector2D* FRectangle::GetBottomLeft() const
{
	return new FVector2D(_xmin, _ymin);
}

FVector2D* FRectangle::GetTopRight() const
{
	return new FVector2D(_xmax, _ymax);
}

FVector2D* FRectangle::GetTopLeft() const
{
	return new FVector2D(_xmin, _ymax);
}

FVector2D* FRectangle::GetBottomRight() const
{
	return new FVector2D(_xmax, _ymin);
}

FVector2D* FRectangle::GetCenter() const
{
	return new FVector2D(_xmin + GetWidth() / 2, _ymin + GetHeight() / 2);
}

const float FRectangle::GetWidth() const
{
	return _xmax - _xmin;
}

const float FRectangle::GetHeight() const
{
	return _ymax - _ymin;
}

std::string FRectangle::ToString()
{
	return std::string(); // ::Format(std::string("[{0},{1}]"), GetBottomLeft(), GetTopRight());
}