#pragma once
#include "GameObject.h"

namespace BRICK_FRAGMENT_PARAMS
{
	constexpr float GRAVITY = 0.001f;
}

// Mảnh gạch bay ra khi gạch bị phá - có trọng lực, tự xóa khi chạm death zone
class BrickFragment : public GameObject
{
private:
	int aniId;  // Animation ID của mảnh gạch
public:
	BrickFragment(float x, float y, float z, float vx, float vy, int aniId) : GameObject(x, y, z) {
		this->vx = vx;
		this->vy = vy;
		this->aniId = aniId;
	};
	virtual ~BrickFragment() {};

	bool IsCollidable() { return false; }
	bool IsBlocking() { return false; }

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
};

