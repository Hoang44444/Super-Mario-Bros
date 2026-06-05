#pragma once
#include "Enemy.h"

#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_GRAVITY 0.002f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 20
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_OTHER  300

#define GOOMBA_DIE_TIMEOUT 500

class Goomba : public Enemy
{
protected:
    DWORD die_start;

public:
    Goomba(float x, float y, float z = 0);

    virtual ~Goomba() {}

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    virtual void SetState(int state);

    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);

    virtual void OnMarioCollison(Mario* mario, float ny) override;
    virtual bool IsCollidable() override;
};