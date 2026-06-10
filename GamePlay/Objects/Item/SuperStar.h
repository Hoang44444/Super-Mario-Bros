#pragma once
#include "Item.h"

constexpr float SUPER_STAR_BBOX_WIDTH    = 16.0f;
constexpr float SUPER_STAR_BBOX_HEIGHT   = 16.0f;
constexpr float SUPER_STAR_GRAVITY       = 0.002f;
constexpr float SUPER_STAR_SPEED         = 0.06f;
constexpr float SUPER_STAR_BOUNCE_SPEED  = 0.45f;

class SuperStar : public Item {
public:
	SuperStar(float x, float y, float z) : Item(x, y, z) {
		vx = SUPER_STAR_SPEED;
		vy = -SUPER_STAR_BOUNCE_SPEED;
		direction = 1;
	}
	virtual ~SuperStar() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	virtual void OnNoCollision(DWORD dt) override;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) override;
	virtual void OnMarioCollision(Mario* mario) override;
};
