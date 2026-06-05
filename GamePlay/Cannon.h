#pragma once
#include "StaticObject.h"

#define CANNON_SHOOT_INTERVAL 2000 // ms

class Cannon : public StaticObject
{
private:
	int direction; // 1: right, -1: left
	DWORD shootTimer;

public:
	Cannon(float x, float y, float z, int dir);
	virtual ~Cannon() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};