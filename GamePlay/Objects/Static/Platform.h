#pragma once
#include "StaticObject.h"

constexpr float PLATFORM_BBOX_WIDTH = 15.0f;
constexpr float PLATFORM_BBOX_HEIGHT = 15.0f;

class Platform : public StaticObject {
public:
	Platform(float x, float y, float z) : StaticObject(x, y, z) {
		this->state = 0;
	}
	virtual ~Platform() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};
