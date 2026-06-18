#include "Bullet.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"
#include "Enemy.h"

void Bullet::Moving(DWORD dt)
{
	// Move along both axes so the bullet follows a parabolic path
	this->x += this->vx * dt;
	this->y += this->vy * dt;
}

void Bullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Gravity makes the trajectory a parabola
	vy += BULLET_GRAVITY * dt;

	Collision::GetInstance()->Process(this, dt, coObjects);

	// Delete once it has travelled far enough from where it was fired
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
	if (e->obj == owner) return;        // Ignore collision with the owner

	if (dynamic_cast<Enemy*>(e->obj)) { // hit an enemy -> handle before the blocking check
		OnCollisionWithEnemy(e);        // (enemies are non-blocking, so this must come first)
		return;
	}

	if (!e->obj->IsBlocking()) return;  // pass through other non-static objects

	// Bounce off static objects instead of disappearing
	if (e->ny < 0) {                    // hit floor -> bounce up
		vy = -BULLET_BOUNCE_SPEED;
	}
	else if (e->ny > 0) {               // hit ceiling -> push down
		vy = BULLET_BOUNCE_SPEED;
	}

	if (e->nx != 0) {                   // hit a wall -> reverse horizontal direction
		vx = -vx;
		direction = -direction;
	}
}

void Bullet::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	if (e->obj == owner) return;        // Ignore collision with the owner
	if (e->obj->IsDeleted()) return;    // Ignore already deleted objects

	// Put the enemy into its die state (each enemy decides how it dies),
	// then remove the bullet.
	if (Enemy* enemy = dynamic_cast<Enemy*>(e->obj)) {
		enemy->OnHitByBullet();
	}
	this->Delete();
}

void Bullet::OnNoCollision(DWORD dt)
{
	Moving(dt);
}

