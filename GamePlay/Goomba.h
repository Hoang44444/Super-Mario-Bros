#pragma once
#include "GameObject.h"


#define GOOMBA_BBOX_WIDTH 18.0f
#define GOOMBA_BBOX_HEIGHT 17.0f
#define GOOMBA_BBOX_HEIGHT_DIE 11.0f

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f


#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200


#define GOOMBA_DIE_TIMEOUT 500

class Goomba : public GameObject
{
protected:
	float ax;
	float ay;
	ULONGLONG die_start;

public:
	Goomba(float x, float y);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual bool IsCollidable() { return true; }
	virtual bool IsBlocking() { return false; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};