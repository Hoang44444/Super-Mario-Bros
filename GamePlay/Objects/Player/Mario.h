#pragma once
#include "GameObject.h"
#include "PlayerData.h"
#include "../../../Resource/SoundManager.h"
#include "../Resource/AssetID.h"
#include "../../Core/ScoreManager.h"
#include "../../Core/MarioPhysics.h"

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
	constexpr DWORD HIT_GRACE_TIME  = 1500;   // brief immunity right after being hurt
	constexpr int   MAX_LIVES       = 3;      // số mạng tối đa

	// Kích thước bounding box theo cấp (neo góc trên-trái). Chiều cao dùng cả ở
	// GetBoundingBox lẫn SetLevel (bù y khi đổi cấp để giữ nguyên chân Mario).
	constexpr float SMALL_BBOX_WIDTH  = 14.0f;
	constexpr float SMALL_BBOX_HEIGHT = 16.0f;
	constexpr float BIG_BBOX_WIDTH    = 14.0f;
	constexpr float BIG_BBOX_HEIGHT   = 28.0f;   // BIG / FIRE / FROG
	
	// Animation hysteresis — B3: ngưỡng thu hẹp giảm nhấp nháy WALK/IDLE khi ma sát
	constexpr float WALK_START_THRESHOLD = 0.03f;   // vx > này -> walk
	constexpr float IDLE_ENTER_THRESHOLD = 0.008f;  // vx < này -> idle
	constexpr float IDLE_EXIT_THRESHOLD  = 0.02f;   // vx > này -> walk từ idle
	constexpr float IDLE_START_THRESHOLD = IDLE_ENTER_THRESHOLD;
	constexpr float RUN_START_THRESHOLD  = 0.13f;   // vx > này + isRunning -> run
	constexpr float WALK_BACK_THRESHOLD  = 0.10f;   // vx < này -> walk (từ run)
	constexpr DWORD ANIM_DEBOUNCE_TIME   = 30;      // B2: ms debounce animation (120ms gây lag)
	constexpr DWORD IDLE_EXIT_DEBOUNCE_TIME = 30;   // 1-2 frame debounce khi thoát idle

	// Ground probe epsilon — khoảng kiểm tra dưới chân Mario để xác định isOnGround
	constexpr float GROUND_PROBE_EPSILON = 0.08f;   // lớn hơn BLOCK_PUSH_FACTOR (0.01f) để đảm bảo detect
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
	bool isInvincible = false;   // miễn thương (dùng cho cả Star lẫn grace sau khi bị hạ cấp)
	bool isStarPower = false;    // riêng Star: chạm enemy là giết enemy
	bool isWindyScene = false;
	DWORD invincibleTime = 0;

	// Animation state machine
	MarioAnimState animState = MarioAnimState::IDLE;
	int animFacing = 1; // 1: right, -1: left
	DWORD animDebounceTimer = 0;
	int lastAnimMoveDir = 0;
	DWORD animReleaseLogTimer = 0;
	DWORD animStopLogTimer = 0;
	bool animStopLogCaptured = false;

	void UpdateAnimationState(DWORD dt);

	// Hệ thống vật lý mới
	MarioPhysics physics;
	
	// Input cho physics system
	MarioPhysicsInput physicsInput;
public:
	Mario(float x, float y, float z) : GameObject(x, y, z) {
		// Khôi phục trạng thái đã giữ qua các màn (level quyết định animation)
		level = PlayerData::Get().level;
		canShoot = (level == MARIO_LEVEL::FIRE);   // khôi phục theo level, nếu không Fire màn mới bấm K không bắn

		animState = MarioAnimState::IDLE;
		animFacing = 1;
		animDebounceTimer = 0;

		// Đảm bảo vật lý bắt đầu từ trạng thái nghỉ khi spawn/màn mới
		physics.Reset();
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
	void MarioBigRender(int& aniId);
	void MarioFireRender(int& aniId);
	void MarioFrogRender(int& aniId);

	// GETTERS AND SETTERS
	void SetSpeedY(float vy) { this->vy = vy; }
	void SetSpeedX(float vx) { this->vx = vx; }
	int GetLevel() { return level; }
	void SetLevel(int level);
	void SetInvincible(DWORD duration) { isInvincible = true; if (duration > invincibleTime) invincibleTime = duration; }
	bool IsInvincible() { return isInvincible; }
	// Star: vừa miễn thương vừa giết enemy khi chạm. Hết giờ thì cả hai tắt (xem Update).
	void SetStarPower(DWORD duration) { isStarPower = true; SetInvincible(duration); }
	bool IsStarPower() { return isStarPower; }
	bool CanShoot() { return canShoot; }
	void SetCanShoot(bool v) { canShoot = v; }
	void AddCoin(int amount = 1) { ScoreManager::Get().AddCoin(amount); }
	void AddLife(int amount = 1) { ScoreManager::Get().AddLife(amount); }
	int GetCoin() { return ScoreManager::Get().GetCoins(); }
	int GetLife() { return ScoreManager::Get().GetLives(); }

	// ĐIỂM SỐ: chỉ là hạ tầng lấy/cộng điểm. Cộng bao nhiêu điểm do từng
	// enemy/item tự quyết và gọi AddScore(...). HUD đọc qua GetScore().
	void AddScore(int amount) { ScoreManager::Get().AddScore(amount); }
	int  GetScore() { return ScoreManager::Get().GetScore(); }

	// WINDY SCENE
	void SetWindyScene(bool isWindy) { this->isWindyScene = isWindy; }

	// PHYSICS INPUT
	void SetPhysicsInput(int moveDir, bool jumpPressed, bool jumpJustPressed, bool runHeld)
	{
		physicsInput.moveDirection = moveDir;
		physicsInput.jumpPressed = jumpPressed;
		physicsInput.jumpJustPressed = jumpJustPressed;
		physicsInput.runHeld = runHeld;
	}

private:
	// coin, life, score are now managed by ScoreManager/PlayerData to ensure persistence
};

