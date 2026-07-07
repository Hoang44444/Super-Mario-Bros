#pragma once

#include "Coin.h"

class ItemCoin2 : public Coin
{
public:
	ItemCoin2(float x, float y, float z) : Coin(x, y, z) {}
	virtual ~ItemCoin2() {}

	void StartEmerge() override;
};
