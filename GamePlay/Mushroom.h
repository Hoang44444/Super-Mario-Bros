#pragma once
#include "Item.h"

#ifndef ID_ANI_MUSHROOM
#define ID_ANI_MUSHROOM 1000
#endif

#define MUSHROOM_GRAVITY		0.002f
#define MUSHROOM_SPEED_X		0.05f

class Mushroom : public Item
{
public:
	Mushroom(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	virtual void Render() override;
	virtual void OnNoCollision(DWORD dt) override;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) override;
};