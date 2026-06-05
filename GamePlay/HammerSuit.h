#pragma once
#include <vector>
#include "Item.h"
#include "../Resource/AssetID.h"

class Mario;

class HammerSuit : public Item
{
public:
	HammerSuit(float x, float y, float z) : Item(x, y, z, ANIMATION::ITEM_HAMMER_SUIT) {}

	virtual ~HammerSuit() {}

	void OnMarioCollision(Mario* mario) override;
};
