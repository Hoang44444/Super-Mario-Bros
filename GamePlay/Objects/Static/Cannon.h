#pragma once
#include "StaticObject.h"

#define CANNON_SHOOT_INTERVAL 10000
#define CANNON_BBOX_WIDTH  16
#define CANNON_BBOX_HEIGHT 32
#define CANNON_MAX_HITS    3

class Cannon : public StaticObject
{
private:
	int direction;
	DWORD shootTimer;
	int hits = 0;

public:
	Cannon(float x, float y, float z, int dir);
	virtual ~Cannon() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	void OnHitByBullet() { if (++hits >= CANNON_MAX_HITS) Delete(); }
};