#pragma once
#include "InvisibleObject.h"

namespace DEATHZONE_BBOX {
	constexpr float WIDTH = 16.0f;
	constexpr float HEIGHT = 16.0f;
}

class DeathZone : public InvisibleObject {
public:
	DeathZone(float x, float y, float z) : InvisibleObject(x, y, z) {};
	virtual ~DeathZone() {};
	virtual void OnMarioCollision(Mario* mario) override;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};

