#pragma once
#include "Enemy.h"

#define BOWSER_WALK_SPEED 0.03f
#define BOWSER_JUMP_SPEED 0.3f
#define BOWSER_GRAVITY 0.001f

#define BOWSER_BBOX_WIDTH 34
#define BOWSER_BBOX_HEIGHT 34

#define BOWSER_STATE_WALKING 100
#define BOWSER_STATE_ATTACKING 200
#define BOWSER_STATE_DIE 300

class Bowser : public Enemy
{
    float startX;
    bool isOnGround = false;

    ULONGLONG jump_timer = 0;
    ULONGLONG fire_timer = 0;
    ULONGLONG attack_duration = 0;
    ULONGLONG current_fire_cooldown = 4000;

    int hp = 5;

public:
    Bowser(float x, float y, float z = 0);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    virtual void SetState(int state);

    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    virtual void OnMarioCollison(Mario* mario, float ny) override;

    virtual bool IsCollidable() override { return state != BOWSER_STATE_DIE; }
};