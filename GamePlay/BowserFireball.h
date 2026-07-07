#pragma once
#include "GameObject.h"

#define BOWSER_FIREBALL_WIDTH 16.0f
#define BOWSER_FIREBALL_HEIGHT 8.0f
#define BOWSER_FIREBALL_SPEED 0.1f

// BowserFireball - đạn lửa của Bowser bay theo chiều ngang
class BowserFireball : public GameObject
{
public:
	BowserFireball(float x, float y, float z, int dir);
	virtual ~BowserFireball() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	virtual bool IsCollidable() override { return true; }
	virtual bool IsBlocking() override { return false; }
};