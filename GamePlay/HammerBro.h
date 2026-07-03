#pragma once
#include "Enemy.h"

#define HAMMER_BRO_WIDTH 16.0f
#define HAMMER_BRO_HEIGHT 24.0f

#define HAMMER_BRO_WALK_SPEED 0.03f
#define HAMMER_BRO_JUMP_SPEED 0.15f
#define HAMMER_BRO_GRAVITY 0.001f

#define HAMMER_BRO_THROW_INTERVAL 1500
#define HAMMER_BRO_JUMP_INTERVAL 3000
#define HAMMER_BRO_WALK_RANGE 32.0f

// Mỗi đợt ném tung ra 10 cây búa, cách nhau một khoảng rất nhỏ (không cùng lúc).
#define HAMMER_BRO_BURST_COUNT 10
#define HAMMER_BRO_BURST_GAP   80

// Khoảng cách tối thiểu (ms) giữa 2 lần quạy đầu khi tì vào tường (tránh lật mỗi frame).
#define HAMMER_BRO_TURN_COOLDOWN 300

// Trạng thái quyết định animation khi Render
#define HAMMER_BRO_STATE_WALK_LEFT  100
#define HAMMER_BRO_STATE_WALK_RIGHT 200
#define HAMMER_BRO_STATE_JUMP       300

class HammerBro : public Enemy
{
private:
	float startX;
	float startY;
	DWORD lastThrowTime;
	DWORD lastJumpTime;
	bool isJumping;

	int   hammersLeft = 0;      // số búa còn lại trong đợt ném hiện tại
	DWORD lastBurstSpawn = 0;   // thời điểm ném cây búa gần nhất trong đợt

public:
	HammerBro(float x, float y, float z);
	virtual ~HammerBro() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	virtual void OnMarioCollison(Mario* mario, float ny) override;

	virtual void OnNoCollision(DWORD dt) override;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) override;

private:
	void ThrowHammer();
	void Jump();
};