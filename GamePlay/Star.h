#pragma once
#include "Item.h"

#ifndef ID_ANI_STAR
#define ID_ANI_STAR 330
#endif

#define STAR_GRAVITY			0.002f
#define STAR_SPEED_X			0.05f
#define STAR_BOUNCE_SPEED		0.25f // Lực nảy lên khi chạm đất

class Star : public Item
{
public:
	Star(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	virtual void Render() override;

	virtual void OnNoCollision(DWORD dt) override;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) override;
};
