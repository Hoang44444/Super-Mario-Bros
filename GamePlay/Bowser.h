#pragma once
#include "Enemy.h"

#define BOWSER_WALK_SPEED 0.03f
#define BOWSER_JUMP_SPEED 0.3f
#define BOWSER_GRAVITY 0.001f

// Chuỗi chết: đứng nhấp nháy tại chỗ, sau đó rơi thẳng (bỏ qua va chạm) rồi tự xóa.
#define BOWSER_DIE_BLINK_TIME 2000       // ms nhấp nháy tại chỗ khi vừa chết
#define BOWSER_DIE_BLINK_INTERVAL 120    // ms mỗi lần chớp sprite

#define BOWSER_BBOX_WIDTH 34
#define BOWSER_BBOX_HEIGHT 34

#define BOWSER_STATE_WALKING 100
#define BOWSER_STATE_ATTACKING 200
#define BOWSER_STATE_DIE 300

class Bowser : public Enemy
{
    float startX;
    bool isOnGround = false;
    bool active = false;   // Mario đã bước lên cầu kích hoạt Bowser chưa

    ULONGLONG jump_timer = 0;
    ULONGLONG fire_timer = 0;
    ULONGLONG attack_duration = 0;
    ULONGLONG current_fire_cooldown = 4000;
    ULONGLONG die_timer = 0;   // thời điểm vào trạng thái chết (để tính pha nhấp nháy 2s)

    int hp = 5;

public:
    Bowser(float x, float y, float z = 0);
	virtual ~Bowser() {}

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    virtual void SetState(int state);

    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    virtual void OnMarioCollison(Mario* mario, float ny) override;

    // Boss takes several hits: each bullet removes one HP, dies only at 0.
    virtual void OnHitByBullet() override { if (--hp <= 0) SetState(BOWSER_STATE_DIE); }

    virtual bool IsCollidable() override { return state != BOWSER_STATE_DIE; }

    // Bowser chỉ đi/nhảy/phun lửa sau khi Mario bước lên cầu. Trước đó đứng yên tại chỗ.
    void Activate()       { active = true; }
    bool IsActive() const { return active; }
};