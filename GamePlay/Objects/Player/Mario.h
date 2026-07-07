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

// Class chính cho nhân vật Mario - xử lý vật lý, animation, va chạm, input
class Mario : public GameObject
{
private:
	int level;  // Level hiện tại của Mario (SMALL, BIG, FIRE, FROG)
	float gravity = MARIO_PARAMS::GRAVITY;  // Trọng lực
	float accelX = MARIO_PARAMS::ACCEL_X;  // Gia tốc X
	void MovementUpdate(DWORD dt);  // Update vị trí dựa trên vận tốc
	void ResolveOverlapWithPlatforms(vector<LPGAMEOBJECT>* coObjects);  // Xử lý overlap với platforms
	bool CheckGroundProbe(vector<LPGAMEOBJECT>* coObjects);  // Kiểm tra Mario có đứng trên đất không
	int CountPlatformOverlaps(vector<LPGAMEOBJECT>* coObjects);  // Đếm số platform Mario đang overlap

	bool isOnGround = false;  // Mario có đang đứng trên đất không
	bool canShoot = false;  // Mario có thể bắn đạn không (Fire level)
	bool isInvincible = false;  // Miễn thương (dùng cho cả Star lẫn grace sau khi bị hạ cấp)
	bool isStarPower = false;  // Riêng Star: chạm enemy là giết enemy
	bool isWindyScene = false;  // Scene có gió không (màn 1-3)
	DynamicPlatform* currentPlatform = nullptr;  // Platform đang đứng trên (vertical hoặc horizontal)
	DWORD invincibleTime = 0;  // Thời gian còn lại của invincibility
	int previousBGM = -1;  // BGM ID trước star power (để khôi phục sau khi star hết)
	float originalMaxRunSpeed = 0.20f;  // Tốc độ chạy gốc trước star power (để khôi phục sau khi star hết)

	// Animation state machine
	MarioAnimState animState = MarioAnimState::IDLE;  // State animation hiện tại
	int animFacing = 1; // 1: phải, -1: trái
	DWORD animDebounceTimer = 0;  // Timer debounce animation
	int lastAnimMoveDir = 0;  // Hướng di chuyển cuối cùng
	DWORD animReleaseLogTimer = 0;  // Timer log khi release movement
	DWORD animStopLogTimer = 0;  // Timer log khi stop movement
	bool animStopLogCaptured = false;  // Đã capture stop log chưa

	void UpdateAnimationState(DWORD dt);  // Update state animation

	// Helper functions for Update()
	void HandleInvincibilityUpdate(DWORD dt);  // Xử lý invincibility timer
	void HandleWindEffect(DWORD dt);  // Xử lý gió kéo Mario
	void HandlePlatformParenting(DWORD dt, vector<LPGAMEOBJECT>* coObjects);  // Xử lý parenting với dynamic platform
	void HandleJumpAfterCollision(bool jumpRequested, bool& jumpedThisFrame, bool wasOnGround);  // Xử lý nhảy sau collision

	// Hệ thống vật lý mới
	MarioPhysics physics;  // Physics engine cho Mario
	
	// Input cho physics system
	MarioPhysicsInput physicsInput;  // Input từ bàn phím cho physics
public:
	Mario(float x, float y, float z) : GameObject(x, y, z) {
		// Khôi phục trạng thái đã giữ qua các màn (level quyết định animation)
		level = PlayerData::Get().level;
		canShoot = (level == MARIO_LEVEL::FIRE);   // Khôi phục theo level, nếu không Fire màn mới bấm K không bắn

		animState = MarioAnimState::IDLE;
		animFacing = 1;
		animDebounceTimer = 0;

		// Đảm bảo vật lý bắt đầu từ trạng thái nghỉ khi spawn/màn mới
		physics.Reset();
	};
	~Mario() {};

	// HÀNH ĐỘNG
	void Jump();  // Nhảy
	void ShootBullet();  // Bắn đạn (Fire level)
	void TakeDamage();  // Bị thương bởi enemy: thu nhỏ thành Small (có grace) hoặc chết nếu đã là Small

	// CORE
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);  // Update Mario mỗi frame
	void Render();  // Render Mario
	void SetState(int state);  // Set state của Mario

	// VA CHẠM
	void GetBoundingBox(float& l, float& t, float& r, float& b);  // Lấy bounding box
	bool IsCollidable() { return true; }  // Mario có thể va chạm
	bool IsBlocking() { return false; }  // Mario không chặn object khác

	void OnCollisionWith(LPCOLLISIONEVENT e);  // Xử lý va chạm chung
	void OnNoCollision(DWORD dt);  // Xử lý khi không có va chạm

	// VA CHẠM VỚI
	void OnCollisionWithStaticObject(LPCOLLISIONEVENT e);  // Va chạm với static object (block, ground)
	void OnCollisionWithDynamicPlatform(LPCOLLISIONEVENT e);  // Va chạm với dynamic platform
	void OnCollisionWithEnemy(LPCOLLISIONEVENT e);  // Va chạm với enemy
	void OnCollisionWithItem(LPCOLLISIONEVENT e);  // Va chạm với item
	void OnCollisionWithInvisibleObject(LPCOLLISIONEVENT e);  // Va chạm với invisible object

	// RENDER THEO LEVEL
	void MarioSmallRender(int& aniId);  // Render Mario nhỏ
	void MarioBigRender(int& aniId);  // Render Mario lớn
	void MarioFireRender(int& aniId);  // Render Mario Fire
	void MarioFrogRender(int& aniId);  // Render Mario Frog

	// GETTERS AND SETTERS
	void SetSpeedY(float vy) { this->vy = vy; }  // Set vận tốc Y
	void SetSpeedX(float vx) { this->vx = vx; }  // Set vận tốc X
	int GetLevel() { return level; }  // Lấy level hiện tại
	void SetLevel(int level);  // Set level (SMALL/BIG/FIRE/FROG)
	void SetInvincible(DWORD duration) { isInvincible = true; if (duration > invincibleTime) invincibleTime = duration; }  // Set invincibility
	bool IsInvincible() { return isInvincible; }  // Check có invincible không
	// Star: vừa miễn thương vừa giết enemy khi chạm. Hết giờ thì cả hai tắt (xem Update).
	void SetStarPower(DWORD duration);  // Set star power
	bool IsStarPower() { return isStarPower; }  // Check có star power không
	bool CanShoot() { return canShoot; }  // Check có thể bắn không
	void SetCanShoot(bool v) { canShoot = v; }  // Set có thể bắn
	void AddCoin(int amount = 1) { ScoreManager::Get().AddCoin(amount); }  // Thêm coin
	void AddLife(int amount = 1) { ScoreManager::Get().AddLife(amount); }  // Thêm mạng
	int GetCoin() { return ScoreManager::Get().GetCoins(); }  // Lấy số coin
	int GetLife() { return ScoreManager::Get().GetLives(); }  // Lấy số mạng

	// ĐIỂM SỐ: chỉ là hạ tầng lấy/cộng điểm. Cộng bao nhiêu điểm do từng
	// enemy/item tự quyết và gọi AddScore(...). HUD đọc qua GetScore().
	void AddScore(int amount) { ScoreManager::Get().AddScore(amount); }  // Thêm điểm
	int  GetScore() { return ScoreManager::Get().GetScore(); }  // Lấy điểm

	// WINDY SCENE
	void SetWindyScene(bool isWindy) { this->isWindyScene = isWindy; }  // Set scene có gió

	// PHYSICS INPUT
	void SetPhysicsInput(int moveDir, bool jumpPressed, bool jumpJustPressed, bool runHeld)  // Set input cho physics
	{
		physicsInput.moveDirection = moveDir;
		physicsInput.jumpPressed = jumpPressed;
		physicsInput.jumpJustPressed = jumpJustPressed;
		physicsInput.runHeld = runHeld;
	}

private:
	// coin, life, score được quản lý bởi ScoreManager/PlayerData để đảm bảo persistence
};

