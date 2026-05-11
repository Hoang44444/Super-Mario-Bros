#pragma once
#pragma once
#include "Item.h"

#ifndef ID_ANI_ONE_UP_MUSHROOM
#define ID_ANI_ONE_UP_MUSHROOM 340
#endif

#define ONE_UP_GRAVITY			0.002f
#define ONE_UP_SPEED_X			0.05f

class OneUpMushroom : public Item
{
public:
	OneUpMushroom(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	virtual void Render() override;

	virtual void OnNoCollision(DWORD dt) override;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) override;
};
