#pragma once
#include "GameObject.h"
#include <cmath>

namespace DYNAMIC_PLATFORM_PARAMS
{
	constexpr float SPEED = 0.05f;
}

namespace DYNAMIC_PLATFORM_TYPE
{
	constexpr int HORIZONTAL = 0;
	constexpr int VERTICAL = 1;
}

namespace DYNAMIC_PLATFORM_BBOX
{
	constexpr float WIDTH = 48.0f;
	constexpr float HEIGHT = 16.0f;
}
class DynamicPlatform : public GameObject{
private: 
	float speed = DYNAMIC_PLATFORM_PARAMS::SPEED;
	float minBound, maxBound;
	float direction = 1; //1: rightn(up), -1: left(down)

	int type; // 0: horizontal, 1: vertical
public:
	DynamicPlatform(float x, float y, float z, int type) : GameObject(x, y, z) {
		this->type = type;

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

	bool IsCollidable() { return true; }
	bool IsBlocking() { return true; }

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void OnNoCollision(DWORD dt);
};

