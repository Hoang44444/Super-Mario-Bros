#pragma once
#include "Item.h"

#ifndef ID_ANI_FIRE_FLOWER
#define ID_ANI_FIRE_FLOWER 320
#endif

class FireFlower : public Item
{
public:
	FireFlower(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	virtual void Render() override;
};