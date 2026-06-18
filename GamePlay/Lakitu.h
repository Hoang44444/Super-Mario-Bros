#pragma once
#include "Enemy.h"

#define LAKITU_FLY_SPEED 0.05f
#define LAKITU_GRAVITY 0.002f 
#define LAKITU_THROW_COOLDOWN 4000

#define LAKITU_BBOX_WIDTH 16
#define LAKITU_BBOX_HEIGHT 24

#define LAKITU_STATE_FLYING 100
#define LAKITU_STATE_DIE 200

class Lakitu : public Enemy
{
    bool isFacingRight;
    float startX;
    ULONGLONG throw_start = 0;

    int max_spinies_on_screen = 3;

public:
    Lakitu(float x, float y, float z = 0);
	
    virtual ~Lakitu() {}

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    virtual void SetState(int state);

    virtual void OnNoCollision(DWORD dt) {}
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    virtual void OnMarioCollison(Mario* mario, float ny) override;
    virtual void OnHitByBullet() override { SetState(LAKITU_STATE_DIE); }
    virtual bool IsCollidable() override;
};
