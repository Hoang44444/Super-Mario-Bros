#pragma once
#include <vector>
#include "Item.h"
#include "../Resource/AssetID.h"

class Mario;

class Coin : public Item
{
public:
	Coin(float x, float y, float z) : Item(x, y, z, ANIMATION::ITEM_COIN) {}

	virtual ~Coin() {}

	void OnMarioCollision(Mario* mario) override;
};
