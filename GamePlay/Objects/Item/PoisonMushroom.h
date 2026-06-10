#pragma once
#include "Item.h"

constexpr float POISON_MUSHROOM_BBOX_WIDTH  = 16.0f;
constexpr float POISON_MUSHROOM_BBOX_HEIGHT = 16.0f;
constexpr float POISON_MUSHROOM_GRAVITY     = 0.002f;
constexpr float POISON_MUSHROOM_SPEED       = 0.05f;

class PoisonMushroom : public Item {
public:
	PoisonMushroom(float x, float y, float z) : Item(x, y, z) {
		vx = POISON_MUSHROOM_SPEED;
		vy = 0;
		direction = 1;
	}
	virtual ~PoisonMushroom() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	virtual void OnNoCollision(DWORD dt) override;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) override;
	virtual void OnMarioCollision(Mario* mario) override;
};
