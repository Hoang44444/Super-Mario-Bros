#pragma once
#include "Item.h"

constexpr float FIRE_FLOWER_BBOX_WIDTH  = 16.0f;
constexpr float FIRE_FLOWER_BBOX_HEIGHT = 16.0f;

class FireFlower : public Item
{
public:
	FireFlower(float x, float y, float z) : Item(x, y, z) {}
	virtual ~FireFlower() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override { UpdateEmerge(dt); }
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	virtual void OnMarioCollision(Mario* mario) override;
};
