#pragma once

#include "NoExportTypes.h"
#include <string>

// Represents rectangle in 2D space.
class FRectangle
{
private:
const float _xmin;
const float _ymin;
const float _xmax;
const float _ymax;

// Initializes a new instance of the <see cref="Rectangle"/> class with predefined bounds.
// @param x"> Minimum x value (left). // 
// @param y"> Minimum y value (bottom). // 
// @param width"> Width of the rectangle. // 
// @param height"> Height of the rectangle. // 
public:
FRectangle(float x, float y, float width, float height);

// Gets left.
const float&  GetLeft() const;

// Gets right.
const float&  GetRight() const;

// Gets bottom.
const float&  GetBottom() const;

// Gets top.
const float&  GetTop() const;

// Gets left bottom point.
FVector2D* GetBottomLeft() const;

// Gets right top point.
FVector2D* GetTopRight() const;

// Gets left top point.
FVector2D* GetTopLeft() const;

// Gets right bottom point.
FVector2D* GetBottomRight() const;

// Gets center point.
FVector2D* GetCenter() const;

// Gets the width of the bounding box.
const float  GetWidth() const;

// Gets the height of the bounding box.
const float  GetHeight() const;


virtual std::string ToString();
};