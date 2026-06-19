#pragma once
#include "GameObject.h"
#include "PlayerData.h"

class DynamicPlatform;


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
	constexpr float GRAVITY         = 0.002f;
	constexpr float ACCEL_X         = 0.0f;
	constexpr float RUN_SPEED       = 0.15f;
	constexpr float WALK_SPEED      = 0.1f;
	constexpr float JUMP_SPEED      = 0.5f;
	constexpr float FROG_JUMP_SPEED   = 0.7f;
	constexpr float FROG_JUMP_SPEED_X = 0.12f;
	constexpr DWORD STAR_POWER_TIME = 7000;
	constexpr DWORD HIT_GRACE_TIME  = 1500;   // brief immunity right after being hurt
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
	bool canShoot = false;
	bool isInvincible = false;
	DWORD invincibleTime = 0;
public:
	Mario(float x, float y, float z) : GameObject(x, y, z) {
		// Khôi phục trạng thái đã giữ qua các màn (level quyết định animation)
		level = PlayerData::Get().level;
		coin  = PlayerData::Get().coins;
		life  = PlayerData::Get().lives;
		canShoot = (level == MARIO_LEVEL::SMALL);   // khôi phục theo level, nếu không Fire màn mới bấm K không bắn
	};
	~Mario() {};

	// ACTIONS
	void Jump();
	void ShootBullet();
	void TakeDamage();   // hurt by an enemy: shrink to Small (with grace) or die if already Small

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
	void MarioFireRender(int& aniId);
	void MarioFrogRender(int& aniId);

	// GETTERS AND SETTERS
	void SetSpeedY(float vy) { this->vy = vy; }
	void SetSpeedX(float vx) { this->vx = vx; }
	int GetLevel() { return level; }
	void SetLevel(int level);
	void SetInvincible(DWORD duration) { isInvincible = true; invincibleTime = duration; }
	bool IsInvincible() { return isInvincible; }
	bool CanShoot() { return canShoot; }
	void SetCanShoot(bool v) { canShoot = v; }
	void AddCoin(int amount = 1) { coin += amount; PlayerData::Get().coins = coin; }
	void AddLife(int amount = 1) { life += amount; PlayerData::Get().lives = life; }
	int GetCoin() { return coin; }
	int GetLife() { return life; }

private:
	int coin = 0;
	int life  = 3;
};

