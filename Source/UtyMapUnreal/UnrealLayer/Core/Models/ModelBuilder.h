#pragma once

#include "Element.h"
#include "Tile.h"
#include "Mesh.h"

class FModelBuilder
{
public:
	void BuildElement(ATile* tile, FElement* element);

	void BuildMesh(ATile* tile, FMesh* mesh);
};