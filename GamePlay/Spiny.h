#pragma once
#include "Enemy.h"

#define SPINY_GRAVITY 0.002f
#define SPINY_WALK_SPEED 0.05f

#define SPINY_BBOX_WIDTH 16
#define SPINY_BBOX_HEIGHT 16

#define SPINY_STATE_WALKING 100
#define SPINY_STATE_EGG 200

#define SPINY_STATE_DIE 300

class Spiny : public Enemy
{
public:
    bool isFacingRight;

    Spiny(float x, float y, float z = 0);
    virtual ~Spiny() {}

    void SetFacingDirection(int dir) { this->direction = dir; }
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    virtual void SetState(int state);

    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    virtual void OnMarioCollison(Mario* mario, float ny) override;
    virtual bool IsCollidable() override;
};