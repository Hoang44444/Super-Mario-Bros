#pragma once
#include "StaticObject.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"

constexpr float BRICK_TEST_BBOX_WIDTH = 16.0f;
constexpr float BRICK_TEST_BBOX_HEIGHT = 16.0f;

// Gạch test - dùng cho debug/testing (render gạch underworld)
class BrickTest : public StaticObject
{
public:
	BrickTest(float x, float y, float z) : StaticObject(x, y, z) {
		this->state = 0;
	}
	virtual ~BrickTest() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) {};
	virtual void Render() {
		AnimationManager::GetInstance()->Get(ANIMATION::BRICK_UNDERWORLD)->Render(x, y, z);
	};
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override{
		l = x;
		t = y;
		r = x + BRICK_TEST_BBOX_WIDTH;
		b = y + BRICK_TEST_BBOX_HEIGHT;
	}
};
