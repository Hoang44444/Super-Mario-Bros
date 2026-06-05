#pragma once
#include "Enemy.h"

#define PODOBOO_GRAVITY 0.001f
#define PODOBOO_JUMP_SPEED 0.45f
#define PODOBOO_WAIT_TIME 2500

#define PODOBOO_BBOX_WIDTH 16
#define PODOBOO_BBOX_HEIGHT 16

#define PODOBOO_STATE_HIDDEN 100
#define PODOBOO_STATE_JUMPING 200

class Podoboo : public Enemy
{
    float startY;
    ULONGLONG wait_start = 0;

public:
    Podoboo(float x, float y, float z = 0);
    virtual ~Podoboo() {}

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    virtual void SetState(int state);

    virtual void OnNoCollision(DWORD dt) {}
    virtual void OnCollisionWith(LPCOLLISIONEVENT e) {}

    virtual void OnMarioCollison(Mario* mario, float ny) override;
};