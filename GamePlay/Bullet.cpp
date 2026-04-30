#include "Bullet.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"

void Bullet::Moving(DWORD dt)
{
	// Move the bullet
	this->x += this->vx * dt;

	// Check if the bullet goes off-screen (Temporarily disabled for debugging)
	if (this->x < 0 || this->x > SCREEN_WIDTH) {
		this->Delete(); 
	}
}

void Bullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void Bullet::Render()
{
	if(this->vx > 0)
		AnimationManager::GetInstance()->Get(ID_ANI_BULLET_RIGHT)->Render(x, y);
	else
		AnimationManager::GetInstance()->Get(ID_ANI_BULLET_LEFT)->Render(x, y);
}

void Bullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BULLET_BBOX_WIDTH;
	b = y + BULLET_BBOX_HEIGHT;
}

void Bullet::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj == owner) return; // Ignore collision with the owner
	if (!e->obj->IsBlocking()) return; // Bullet doesn't block, so it can pass through objects

	this->Delete(); // Mark for deletion on collision
}

void Bullet::OnNoCollision(DWORD dt)
{
	Moving(dt);
}

