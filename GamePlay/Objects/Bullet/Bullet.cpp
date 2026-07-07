#include "Bullet.h"
#include "AnimationManager.h"
#include "AssetID.h"
#include "debug.h"
#include "Enemy.h"
#include "Cannon.h"

void Bullet::Moving(DWORD dt)
{
	this->x += this->vx * dt;
	this->y += this->vy * dt;
}

void Bullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += BULLET_GRAVITY * dt;

	Collision::GetInstance()->Process(this, dt, coObjects);

	if (abs(this->x - startX) > BULLET_MAX_DISTANCE) {
		this->Delete();
	}
}

void Bullet::Render()
{
	int aniId = ANIMATION::BULLET_RIGHT;
	if (this->direction < 0) {
		aniId = ANIMATION::BULLET_LEFT;
	}
	AnimationManager::GetInstance()->Get(aniId)->Render(x, y, z);
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
	if (e->obj == owner) return;

	if (dynamic_cast<Enemy*>(e->obj)) {
		OnCollisionWithEnemy(e);
		return;
	}

	if (Cannon* cannon = dynamic_cast<Cannon*>(e->obj)) {
		cannon->OnHitByBullet();
		this->Delete();
		return;
	}

	if (!e->obj->IsBlocking()) return;

	if (e->nx != 0) {
		this->Delete();
		return;
	}

	if (e->ny < 0) {
		vy = -BULLET_BOUNCE_SPEED;
	}
	else if (e->ny > 0) {
		vy = BULLET_BOUNCE_SPEED;
	}
}

void Bullet::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	if (e->obj == owner) return;
	if (e->obj->IsDeleted()) return;

	if (Enemy* enemy = dynamic_cast<Enemy*>(e->obj)) {
		enemy->OnHitByBullet();
	}
	this->Delete();
}

void Bullet::OnNoCollision(DWORD dt)
{
	Moving(dt);
}

