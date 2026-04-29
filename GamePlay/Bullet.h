#pragma once
#include "GameObject.h"

#define BULLET_SPEED 0.2f
#define BULLET_BBOX_WIDTH 8
#define BULLET_BBOX_HEIGHT 8
#define SCREEN_WIDTH 800.00f


class Bullet : public GameObject
{
protected:
	LPGAMEOBJECT owner; // The object that fired the bullet
public:
	Bullet(float x, float y, int direction, LPGAMEOBJECT owner) : GameObject(x, y) {
		this->direction = direction;
		this->vx = BULLET_SPEED * direction; // Speed of the bullet
		this->owner = owner;
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
};

