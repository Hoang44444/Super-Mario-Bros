#pragma once
#include "Enemy.h"

#define BOWSER_FIRE_SPEED 0.15f
#define BOWSER_FIRE_BBOX_WIDTH 24
#define BOWSER_FIRE_BBOX_HEIGHT 8

class BowserFire : public Enemy
{
    float startX;
public:
    BowserFire(float x, float y, float z, int direction);
	virtual ~BowserFire() {}

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

    virtual void OnNoCollision(DWORD dt) { x += vx * dt; y += vy * dt; }
    virtual void OnCollisionWith(LPCOLLISIONEVENT e) {}

    virtual void OnMarioCollison(Mario* mario, float ny) override;
};