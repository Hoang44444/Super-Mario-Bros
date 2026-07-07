#pragma once
#include "Item.h"
#include <vector>
#include "../Resource/AssetID.h"

constexpr float MUSHROOM_BBOX_WIDTH  = 16.0f;
constexpr float MUSHROOM_BBOX_HEIGHT = 16.0f;
constexpr float MUSHROOM_GRAVITY     = 0.002f;
constexpr float MUSHROOM_SPEED       = 0.05f;

// Nấm Super - Mario ăn sẽ lớn lên (Small -> Big)
class Mushroom : public Item
{
public:
	Mushroom(float x, float y, float z) : Item(x, y, z) {
		vx = MUSHROOM_SPEED;
		vy = 0;
		direction = 1;
	}
	virtual ~Mushroom() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	virtual void OnNoCollision(DWORD dt) override;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) override;
	virtual void OnMarioCollision(Mario* mario) override;

protected:
	void OnEmergeComplete() override { vx = MUSHROOM_SPEED * direction; }  // Bắt đầu đi sau khi nhảy xong
};
