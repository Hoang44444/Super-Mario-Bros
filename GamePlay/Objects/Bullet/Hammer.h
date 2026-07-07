#pragma once
#include "GameObject.h"

#define HAMMER_WIDTH 8.0f
#define HAMMER_HEIGHT 8.0f
#define HAMMER_GRAVITY 0.001f
#define HAMMER_SPEED_X 0.08f
#define HAMMER_SPEED_Y 0.2f

class Hammer : public GameObject
{
public:
	Hammer(float x, float y, float z, int dir);
	virtual ~Hammer() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	virtual bool IsCollidable() override { return true; }
	virtual bool IsBlocking() override { return false; }
};