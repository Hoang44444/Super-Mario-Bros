#pragma once
#include "GameObject.h"

class DynamicPlatform;

namespace MARIO_LEVEL
{
	constexpr int SMALL = 1;
	constexpr int BIG   = 2;
}

namespace MARIO_STATE
{
	constexpr int DIE           = -10;
	constexpr int IDLE          = 0;

	constexpr int WALKING_RIGHT = 100;
	constexpr int WALKING_LEFT  = 200;

	constexpr int RUNNING_RIGHT = 110;
	constexpr int RUNNING_LEFT  = 210;

	constexpr int JUMP          = 300;
	constexpr int RELEASE_JUMP  = 301;

	constexpr int SIT           = 400;
	constexpr int SIT_RELEASE   = 401;

	constexpr int SHOOT         = 500;
}

namespace MARIO_PARAMS
{
	constexpr float GRAVITY    = 0.002f;
	constexpr float ACCEL_X    = 0.0f;
	constexpr float RUN_SPEED  = 0.15f;
	constexpr float WALK_SPEED = 0.1f;
	constexpr float JUMP_SPEED = 0.5f;
}

class Mario : public GameObject
{
private:
	int level; 
	float gravity = MARIO_PARAMS::GRAVITY;
	float accelX = MARIO_PARAMS::ACCEL_X;
	void MovementUpdate(DWORD dt);
	void ResolveOverlapWithPlatforms(vector<LPGAMEOBJECT>* coObjects);

	bool isOnGround = false;
public:
	Mario(float x, float y, float z) : GameObject(x, y, z) {
		level = MARIO_LEVEL::BIG;
	};
	~Mario() {};

	// ACTIONS
	void Jump();
	void ShootBullet();

	// CORE
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);

	// COLLISION
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	bool IsCollidable() { return true; }
	bool IsBlocking() { return false; }

	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt);

	// COLISION WITH
	void OnCollisionWithStaticObject(LPCOLLISIONEVENT e);
	void OnCollisionWithDynamicPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithEnemy(LPCOLLISIONEVENT e);
	void OnCollisionWithItem(LPCOLLISIONEVENT e);
	void OnCollisionWithInvisibleObject(LPCOLLISIONEVENT e);

	// RENDER WITH MARIO LEVEL
	void MarioSmallRender(int& aniId);
	void MarioBigRender(int& andId);

	// GETTERS AND SETTERS
	void SetSpeedY(float vy) { this->vy = vy; }
	void SetSpeedX(float vx) { this->vx = vx; }
	int GetLevel() { return level; }
};

