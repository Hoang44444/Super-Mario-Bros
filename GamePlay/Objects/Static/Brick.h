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
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override; 
	void Render() override ;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void OnMarioCollision(Mario* mario, LPCOLLISIONEVENT e) override;
};

