#pragma once
#include "Enemy.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALK_SPEED 0.05f
#define KOOPA_SPIN_SPEED 0.2f

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_BBOX_HEIGHT_SHELL 20

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_SHELL 200
#define KOOPA_STATE_SPINNING 300

#define KOOPA_STATE_DIE 400

#define KOOPA_SHELL_TIMEOUT 5000

class Koopa : public Enemy
{
public:
    ULONGLONG shell_start = 0;
    bool isFacingRight;

    Koopa(float x, float y, float z = 0);
    virtual ~Koopa() {}

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    virtual void SetState(int state);

    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    virtual void OnMarioCollison(Mario* mario, float ny) override;
    virtual bool IsCollidable() override;
};
