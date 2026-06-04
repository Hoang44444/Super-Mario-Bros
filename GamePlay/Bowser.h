#pragma once
#include "Enemy.h"

#define BOWSER_WIDTH 32.0f
#define BOWSER_HEIGHT 32.0f

#define BOWSER_WALK_SPEED 0.02f
#define BOWSER_JUMP_SPEED 0.12f
#define BOWSER_GRAVITY 0.001f

#define BOWSER_WALK_RANGE 48.0f
#define BOWSER_SHOOT_INTERVAL 3000
#define BOWSER_JUMP_INTERVAL 4000

class Bowser : public Enemy
{
private:
	float startX;
	float startY;
	int hp;
	DWORD lastShootTime;
	DWORD lastJumpTime;
	bool isJumping;

public:
	Bowser(float x, float y, float z);
	virtual ~Bowser() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	virtual void OnMarioCollison(Mario* mario) override;

	virtual void OnNoCollision(DWORD dt) override;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void TakeDamage(int damage);

private:
	void ShootFire();
	void Jump();
};