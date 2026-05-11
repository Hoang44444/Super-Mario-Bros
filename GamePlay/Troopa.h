#pragma once
#include "GameObject.h"

// Physics and Dimensions
#define TROOPA_GRAVITY 0.002f
#define TROOPA_WALKING_SPEED 0.05f
#define TROOPA_SHELL_SPEED 0.15f

#define TROOPA_BBOX_WIDTH 16.0f
#define TROOPA_BBOX_HEIGHT 26.0f
#define TROOPA_BBOX_HEIGHT_SHELL 14.0f

// States
#define TROOPA_STATE_WALKING 100
#define TROOPA_STATE_SHELL 200
#define TROOPA_STATE_SHELL_MOVING 300
#define TROOPA_STATE_DIE 400

class Troopa : public GameObject
{
protected:
	float ax;
	float ay;
	ULONGLONG die_start;

public:
	Troopa(float x, float y);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual bool IsCollidable() { return (state != TROOPA_STATE_DIE); }
	virtual bool IsBlocking() { return false; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	// Custom methods for Troopa logic
	int GetState() { return this->state; }
	void SetVx(float vx) { this->vx = vx; }
};