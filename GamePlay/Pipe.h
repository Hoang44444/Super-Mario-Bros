#pragma once
#include "GameObject.h"

constexpr float PIPE_BBOX_WIDTH = 32.0f;
constexpr float PIPE_BBOX_HEIGHT = 32.0f;

class Pipe : public GameObject {
public:
	Pipe(float x, float y, float z) : GameObject(x, y, z) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override {}
	virtual void Render() override {};
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override
	{
		l = x;
		t = y;
		r = x + PIPE_BBOX_WIDTH;
		b = y + PIPE_BBOX_HEIGHT;
	}
};