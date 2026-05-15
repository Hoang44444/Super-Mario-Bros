#pragma once
#include "GameObject.h"

constexpr float BRICK_BBOX_WIDTH = 16.0f;
constexpr float BRICK_BBOX_HEIGHT = 16.0f;

class Brick : public GameObject {
public:
	Brick(float x, float y, float z) : GameObject(x, y, z) {
		this->state = 0;
	}
	~Brick() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() {};
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	bool IsCollidable() override { return true; }
	bool IsBlocking() override { return true; }

};

