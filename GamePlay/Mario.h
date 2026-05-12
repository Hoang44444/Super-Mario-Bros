#pragma once
#include "GameObject.h"

// MARIO LEVELS
#define MARIO_LEVEL_SMALL	1
#define MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_FIRE	3

// MARIO STATES 
#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0

#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_RUNNING_RIGHT	110
#define MARIO_STATE_RUNNING_LEFT	210

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_SIT				400
#define MARIO_STATE_SIT_RELEASE		401

#define MARIO_STATE_SHOOT			500

// PARAMETERS
#define MARIO_GRAVITY				0.002f
#define MARIO_ACCEL_RUN				0.0005f
#define MARIO_ACCEL_WALK			0.0004f
#define MARIO_VELOCITY_WALK_MAX		0.05f
#define MARIO_VELOCITY_RUN_MAX		0.15f
#define MARIO_JUMP_SPEED			0.7f
#define MARIO_INVINCIBLE_BLINK_INTERVAL 100
#define MARIO_INVICIBLE_TIME 5000

class Mario : public GameObject
{
private:
	int level; 
	float gravity = MARIO_GRAVITY;
	float accelX = 0;
	int score = 0;
	int lives = 3;
	bool isInvincible = false;
	bool isOnGround = false; 
	ULONGLONG untouchable_start = 0;
public:
	Mario(float x, float y) : GameObject(x, y) {
		level = MARIO_LEVEL_SMALL;
	}
	~Mario() {};
	// ACTIONS
	void MovementUpdate(DWORD dt);
	void ShootBullet();
	void InvincibleAction();

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

	//Getter/Setter
	int GetLevel() { return this->level; }
	void SetLevel(int l) { this->level = l; }
	void AddScore(int s) { this->score += s; }
};

