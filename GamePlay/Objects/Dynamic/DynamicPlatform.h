#pragma once
#include "GameObject.h"
#include <cmath>

namespace DYNAMIC_PLATFORM_PARAMS
{
	constexpr float SPEED = 0.01f;
}

namespace DYNAMIC_PLATFORM_TYPE
{
	constexpr int HORIZONTAL = 0;
	constexpr int VERTICAL = 1;
}

namespace DYNAMIC_PLATFORM_BBOX
{
	constexpr float WIDTH = 48.0f;
	constexpr float HEIGHT = 32.0f;
}
class DynamicPlatform : public GameObject{
private: 
	float speed = DYNAMIC_PLATFORM_PARAMS::SPEED;
	float minBound, maxBound;
	float direction = 1; //1: rightn(down), -1: left(up)

	int type; // 0: horizontal, 1: vertical
public:
	DynamicPlatform(float x, float y, float z, int type, int initialDirection = 1) : GameObject(x, y, z) {
		this->type = type;
		this->direction = initialDirection;

		if (type == DYNAMIC_PLATFORM_TYPE::HORIZONTAL) {
			this->minBound = x - 50;
			this->maxBound = x + 50;
		}
		else {
			this->minBound = y - 50;
			this->maxBound = y + 50;
		}
	};

	virtual ~DynamicPlatform() {};

	void Movement(DWORD dt);

	bool IsCollidable() { return true; }
	bool IsBlocking() { return true; }

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void OnNoCollision(DWORD dt) override;

	float GetVx() const { return type == DYNAMIC_PLATFORM_TYPE::HORIZONTAL ? speed * direction : 0.0f; }
	float GetVy() const { return type == DYNAMIC_PLATFORM_TYPE::VERTICAL ? speed * direction : 0.0f; }
};

