#pragma once
#include "Item.h"

class Coin : public Item
{
public:
	Coin(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	virtual void Render() override;
};