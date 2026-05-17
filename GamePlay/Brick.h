#pragma once
#include "StaticObject.h"

constexpr float BRICK_BBOX_WIDTH = 16.0f;
constexpr float BRICK_BBOX_HEIGHT = 16.0f;

class Brick : public StaticObject {
public:
	Brick(float x, float y, float z) : StaticObject(x, y, z) {
		this->state = 0;
	}
	virtual ~Brick() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() {};
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override{
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	}
};

