#pragma once
#include <vector>
#include "Item.h"
#include "../Resource/AssetID.h"

class Mario;

class SuperLeaf : public Item
{
public:
	SuperLeaf(float x, float y, float z) : Item(x, y, z, ANIMATION::ITEM_SUPER_LEAF) {}

	virtual ~SuperLeaf() {}

	void OnMarioCollision(Mario* mario) override;
};
