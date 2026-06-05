#pragma once
#include <vector>
#include "Item.h"
#include "../Resource/AssetID.h"

class Mario;

class TanookiSuit : public Item
{
public:
	TanookiSuit(float x, float y, float z) : Item(x, y, z, ANIMATION::ITEM_TANOOKI_SUIT) {}

	virtual ~TanookiSuit() {}

	void OnMarioCollision(Mario* mario) override;
};
