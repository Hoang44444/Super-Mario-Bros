#pragma once
#include "Enemy.h"

#define BUZZY_GRAVITY 0.002f
#define BUZZY_WALK_SPEED 0.05f
#define BUZZY_SPIN_SPEED 0.2f

#define BUZZY_BBOX_WIDTH 18
#define BUZZY_BBOX_HEIGHT 18
#define BUZZY_BBOX_HEIGHT_SHELL 16 

#define BUZZY_STATE_WALKING 100
#define BUZZY_STATE_SHELL 200
#define BUZZY_STATE_SPINNING 300

#define BUZZY_STATE_DIE 400

#define BUZZY_SHELL_TIMEOUT 5000

class BuzzyBeetle : public Enemy
{
public:
    ULONGLONG shell_start = 0;
    bool isFacingRight;

    BuzzyBeetle(float x, float y, float z = 0);
    virtual ~BuzzyBeetle() {}

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    virtual void SetState(int state);

    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    virtual void OnMarioCollison(Mario* mario, float ny) override;
    virtual void OnHitByBullet() override { SetState(BUZZY_STATE_DIE); }
    // Only a normally-walking BuzzyBeetle is flipped by a turn block; a spinning shell is not.
    virtual bool CanBeTurnedByBlock() override { return state == BUZZY_STATE_WALKING; }
    virtual bool IsCollidable() override;
};