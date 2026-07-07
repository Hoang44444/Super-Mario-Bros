#pragma once
#include "GameObject.h"

#define BULLET_SPEED 0.2f
#define BULLET_GRAVITY 0.0008f
#define BULLET_BOUNCE_SPEED 0.25f
#define BULLET_MAX_DISTANCE 400.0f
#define BULLET_BBOX_WIDTH 8
#define BULLET_BBOX_HEIGHT 8
#define SCREEN_WIDTH 800.00f


class Bullet : public GameObject
{
protected:
	LPGAMEOBJECT owner;
	float startX;
public:
	Bullet(float x, float y, int direction, LPGAMEOBJECT owner) : GameObject(x, y, z) {
		this->z = 0.5f;
		this->direction = direction;
		this->vx = BULLET_SPEED * direction;
		this->vy = 0;
		this->owner = owner;
		this->startX = x;
	};
	~Bullet() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	bool IsCollidable() { return true; }
	bool IsBlocking() { return false; }
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt);
	void Moving(DWORD dt);
	void OnCollisionWithEnemy(LPCOLLISIONEVENT e);

};

