#pragma once
#include "GameObject.h"
#include "PlayerData.h"
#include "SoundManager.h"
#include "AssetID.h"
#include "ScoreManager.h"
#include "MarioPhysics.h"

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

enum class MarioAnimState
{
	IDLE,
	WALKING,
	RUNNING,
	SKIDDING,
	JUMPING,
	SITTING,
	DYING
};

namespace MARIO_PARAMS
{
	constexpr float GRAVITY         = 0.0021f;
	constexpr float ACCEL_X         = 0.0f;
	constexpr float RUN_SPEED       = 0.15f;
	constexpr float WALK_SPEED      = 0.15f;
	constexpr float JUMP_SPEED      = 0.55f;
	constexpr float FROG_JUMP_SPEED   = 0.7f;
	constexpr float FROG_JUMP_SPEED_X = 0.12f;
	constexpr DWORD STAR_POWER_TIME = 7000;
	constexpr DWORD HIT_GRACE_TIME  = 1500;
	constexpr int   MAX_LIVES       = 3;

	constexpr float SMALL_BBOX_WIDTH  = 14.0f;
	constexpr float SMALL_BBOX_HEIGHT = 16.0f;
	constexpr float BIG_BBOX_WIDTH    = 14.0f;
	constexpr float BIG_BBOX_HEIGHT   = 28.0f;

	constexpr float WALK_START_THRESHOLD = 0.03f;
	constexpr float IDLE_ENTER_THRESHOLD = 0.008f;
	constexpr float IDLE_EXIT_THRESHOLD  = 0.02f;
	constexpr float IDLE_START_THRESHOLD = IDLE_ENTER_THRESHOLD;
	constexpr float RUN_START_THRESHOLD  = 0.13f;
	constexpr float WALK_BACK_THRESHOLD  = 0.10f;
	constexpr DWORD ANIM_DEBOUNCE_TIME   = 30;
	constexpr DWORD IDLE_EXIT_DEBOUNCE_TIME = 30;

	constexpr float GROUND_PROBE_EPSILON = 0.08f;
}

class Mario : public GameObject
{
private:
	int level;
	float gravity = MARIO_PARAMS::GRAVITY;
	float accelX = MARIO_PARAMS::ACCEL_X;
	void MovementUpdate(DWORD dt);
	void ResolveOverlapWithPlatforms(vector<LPGAMEOBJECT>* coObjects);
	bool CheckGroundProbe(vector<LPGAMEOBJECT>* coObjects);

	bool isOnGround = false;
	bool canShoot = false;
	bool isInvincible = false;
	bool isStarPower = false;
	bool isWindyScene = false;
	bool flyMode = false;
	DynamicPlatform* currentPlatform = nullptr;
	DWORD invincibleTime = 0;
	int previousBGM = -1;
	float originalMaxRunSpeed = 0.20f;

	MarioAnimState animState = MarioAnimState::IDLE;
	int animFacing = 1;
	DWORD animDebounceTimer = 0;
	int lastAnimMoveDir = 0;
	DWORD animReleaseLogTimer = 0;
	DWORD animStopLogTimer = 0;
	bool animStopLogCaptured = false;

	void UpdateAnimationState(DWORD dt);

	MarioPhysics physics;

	MarioPhysicsInput physicsInput;
public:
	Mario(float x, float y, float z) : GameObject(x, y, z) {
		level = PlayerData::Get().level;
		canShoot = (level == MARIO_LEVEL::FIRE);

		animState = MarioAnimState::IDLE;
		animFacing = 1;
		animDebounceTimer = 0;

		physics.Reset();
	};
	~Mario() {};

	void Jump();
	void ShootBullet();
	void TakeDamage();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	bool IsCollidable() { return true; }
	bool IsBlocking() { return false; }

	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt);

	void OnCollisionWithStaticObject(LPCOLLISIONEVENT e);
	void OnCollisionWithDynamicPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithEnemy(LPCOLLISIONEVENT e);
	void OnCollisionWithItem(LPCOLLISIONEVENT e);
	void OnCollisionWithInvisibleObject(LPCOLLISIONEVENT e);

	void MarioSmallRender(int& aniId);
	void MarioBigRender(int& aniId);
	void MarioFireRender(int& aniId);
	void MarioFrogRender(int& aniId);

	void SetSpeedY(float vy) { this->vy = vy; }
	void SetSpeedX(float vx) { this->vx = vx; }
	int GetLevel() { return level; }
	void SetLevel(int level);
	void SetInvincible(DWORD duration) { isInvincible = true; if (duration > invincibleTime) invincibleTime = duration; }
	bool IsInvincible() { return isInvincible; }
	void SetStarPower(DWORD duration);
	bool IsStarPower() { return isStarPower; }
	bool CanShoot() { return canShoot; }
	void SetCanShoot(bool v) { canShoot = v; }
	void SetFlyMode(bool v) { flyMode = v; }
	bool IsFlyMode() { return flyMode; }
	void AddCoin(int amount = 1) { ScoreManager::Get().AddCoin(amount); }
	void AddLife(int amount = 1) { ScoreManager::Get().AddLife(amount); }
	int GetCoin() { return ScoreManager::Get().GetCoins(); }
	int GetLife() { return ScoreManager::Get().GetLives(); }

	void AddScore(int amount) { ScoreManager::Get().AddScore(amount); }
	int  GetScore() { return ScoreManager::Get().GetScore(); }

	void SetWindyScene(bool isWindy) { this->isWindyScene = isWindy; }

	void SetPhysicsInput(int moveDir, bool jumpPressed, bool jumpJustPressed, bool runHeld)
	{
		physicsInput.moveDirection = moveDir;
		physicsInput.jumpPressed = jumpPressed;
		physicsInput.jumpJustPressed = jumpJustPressed;
		physicsInput.runHeld = runHeld;
	}

private:
};

