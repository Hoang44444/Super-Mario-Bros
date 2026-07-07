#pragma once
#include "Item.h"

constexpr float AXE_BBOX_WIDTH  = 16.0f;
constexpr float AXE_BBOX_HEIGHT = 16.0f;

class Axe : public Item
{
public:
	Axe(float x, float y, float z) : Item(x, y, z) {}
	virtual ~Axe() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override {}
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	virtual void OnMarioCollision(Mario* mario) override;
};
