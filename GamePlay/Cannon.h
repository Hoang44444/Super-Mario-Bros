#pragma once
#include "StaticObject.h"

#define CANNON_SHOOT_INTERVAL 10000 // ms
#define CANNON_BBOX_WIDTH  16
#define CANNON_BBOX_HEIGHT 32
#define CANNON_MAX_HITS    3        // số phát đạn Mario cần bắn trúng để phá huỷ

class Cannon : public StaticObject
{
private:
	int direction; // 1: right, -1: left
	ULONGLONG shootTimer;
	int hits = 0;  // số lần đã trúng đạn Mario

public:
	Cannon(float x, float y, float z, int dir);
	virtual ~Cannon() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	// Trúng đạn Mario: đủ CANNON_MAX_HITS lần thì biến mất (chặn Mario tới khi bị phá).
	void OnHitByBullet() { if (++hits >= CANNON_MAX_HITS) Delete(); }
};