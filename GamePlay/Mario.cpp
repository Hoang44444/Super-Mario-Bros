#include "Mario.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"
#include "Bullet.h"
#include "StaticObject.h"
#include "Enemy.h"
#include "Item.h"
#include "InvisibleObject.h"
void Mario::MovementUpdate(DWORD dt) {
	// Simple movement for testing
	this->x += this->vx * dt;
	this->y += this->vy * dt;

	this->vx += this->accelX * dt;
	this->vy += this->gravity * dt;
}

void Mario::ShootBullet() { 
	float bulletX = x + (direction > 0 ? 15.0f : -8.0f);
	float bulletY = y;
	scene->AddObject(new Bullet(bulletX, bulletY, direction, this));
}

void Mario::SetLevel(int level)
{
	this->level = level;
	this->canShoot = level == MARIO_LEVEL_FIRE;
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isInvincible)
	{
		if (invincibleTime > dt)
			invincibleTime -= dt;
		else
		{
			invincibleTime = 0;
			isInvincible = false;
		}
	}

	DebugOut(L"[MARIO POSITION] x=%f y=%f\n", x, y);
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void Mario::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		direction = 1;
		if (level == MARIO_LEVEL_FROG)
			vx = isOnGround ? 0 : MARIO_FROG_JUMP_SPEED_X;
		else
			vx = MARIO_WALK_SPEED;
		break;
	case MARIO_STATE_WALKING_LEFT:
		direction = -1;
		if (level == MARIO_LEVEL_FROG)
			vx = isOnGround ? 0 : -MARIO_FROG_JUMP_SPEED_X;
		else
			vx = -MARIO_WALK_SPEED;
		break;
	case MARIO_STATE_JUMP:
		if (isOnGround) {
			if (level == MARIO_LEVEL_FROG)
			{
				vy = -MARIO_FROG_JUMP_SPEED;
				vx = MARIO_FROG_JUMP_SPEED_X * direction;
			}
			else
			{
				vy = -MARIO_JUMP_SPEED;
			}
			isOnGround = false;
		}
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_SPEED;
		break;
	case MARIO_STATE_SHOOT:
		if (canShoot)
			ShootBullet();
		this->state = MARIO_STATE_IDLE; // Revert to idle so Mario doesn't disappear
		break;
	case MARIO_STATE_RUNNING_LEFT:
		vx = -MARIO_RUN_SPEED;
		direction = -1;
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		vx = MARIO_RUN_SPEED;
		direction = 1;
		break;
	}
}

void Mario::MarioSmallRender(int& aniId) {
	if (!isOnGround) {
		if (direction > 0) aniId = ANIMATION::MARIO_SMALL_JUMP_WALK_RIGHT;
		else aniId = ANIMATION::MARIO_SMALL_JUMP_WALK_LEFT;
	}
	else if (vx != 0) {
		if (direction > 0) aniId = ANIMATION::MARIO_SMALL_WALKING_RIGHT;
		else aniId = ANIMATION::MARIO_SMALL_WALKING_LEFT;
	}
	else {
		if (direction > 0) aniId = ANIMATION::MARIO_SMALL_IDLE_RIGHT;
		else aniId = ANIMATION::MARIO_SMALL_IDLE_LEFT;
	}
}

void Mario::MarioBigRender(int& aniId) {
	if (!isOnGround) {
		if (direction > 0) aniId = ANIMATION::MARIO_BIG_JUMP_WALK_RIGHT;
		else aniId = ANIMATION::MARIO_BIG_JUMP_WALK_LEFT;
	}
	else if (vx != 0) {
		if (direction > 0) aniId = ANIMATION::MARIO_BIG_WALKING_RIGHT;
		else aniId = ANIMATION::MARIO_BIG_WALKING_LEFT;
	}
	else {
		if (direction > 0) aniId = ANIMATION::MARIO_BIG_IDLE_RIGHT;
		else aniId = ANIMATION::MARIO_BIG_IDLE_LEFT;
	}
}

void Mario::MarioFireRender(int& aniId) {
	if (!isOnGround) {
		if (direction > 0) aniId = ANIMATION::MARIO_FIRE_JUMP_WALK_RIGHT;
		else aniId = ANIMATION::MARIO_FIRE_JUMP_WALK_LEFT;
	}
	else if (vx != 0) {
		if (direction > 0) aniId = ANIMATION::MARIO_FIRE_WALKING_RIGHT;
		else aniId = ANIMATION::MARIO_FIRE_WALKING_LEFT;
	}
	else {
		if (direction > 0) aniId = ANIMATION::MARIO_FIRE_IDLE_RIGHT;
		else aniId = ANIMATION::MARIO_FIRE_IDLE_LEFT;
	}
}

void Mario::MarioFrogRender(int& aniId) {
	if (!isOnGround) {
		if (direction > 0) aniId = ANIMATION::MARIO_FROG_JUMP_RIGHT;
		else aniId = ANIMATION::MARIO_FROG_JUMP_LEFT;
	}
	else if (vx != 0) {
		if (direction > 0) aniId = ANIMATION::MARIO_FROG_WALKING_RIGHT;
		else aniId = ANIMATION::MARIO_FROG_WALKING_LEFT;
	}
	else {
		if (direction > 0) aniId = ANIMATION::MARIO_FROG_IDLE_RIGHT;
		else aniId = ANIMATION::MARIO_FROG_IDLE_LEFT;
	}
}

void Mario::Render()
{
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ANIMATION::MARIO_DIE;
	else if (level == MARIO_LEVEL_FIRE)
	{
		MarioFireRender(aniId);
	}
	else if (level == MARIO_LEVEL_FROG)
	{
		MarioFrogRender(aniId);
	}
	else if (level == MARIO_LEVEL_BIG)
	{
		MarioBigRender(aniId);
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		MarioSmallRender(aniId);
	}

	if (aniId != -1)
		AnimationManager::GetInstance()->Get(aniId)->Render(x, y, z);
}

void Mario::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE || level == MARIO_LEVEL_FROG)
	{
		r = x + 15;
		b = y + 27;
	}
	else
	{
		r = x + 13;
		b = y + 15;
	}
}

void Mario::OnCollisionWithStaticObject(LPCOLLISIONEVENT e)
{
	if (e->ny < 0) {
		isOnGround = true;
		vy = 0;
	}
	else if (e->nx != 0) {
		vx = 0;
	}
}

void Mario::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	if (isInvincible)
	{
		e->obj->Delete();
		return;
	}

	auto enemy = dynamic_cast<Enemy*>(e->obj);
	enemy->OnMarioCollison(this);
}

void Mario::OnCollisionWithItem(LPCOLLISIONEVENT e)
{
	auto item = dynamic_cast<Item*>(e->obj);
	item->OnMarioCollision(this);
}

void Mario::OnCollisionWithInvisibleObject(LPCOLLISIONEVENT e)
{
	auto invisibleObject = dynamic_cast<InvisibleObject*>(e->obj);
	invisibleObject->OnMarioCollision(this);
}

void Mario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<StaticObject*>(e->obj)) {
		OnCollisionWithStaticObject(e);
	}
	else if (dynamic_cast<Enemy*>(e->obj)) {
		OnCollisionWithEnemy(e);
	}
	else if (dynamic_cast<Item*>(e->obj)) {
		OnCollisionWithItem(e);
	}
	else if (dynamic_cast<InvisibleObject*>(e->obj)) {
		OnCollisionWithInvisibleObject(e);
	}
}

void Mario::OnNoCollision(DWORD dt)
{
	MovementUpdate(dt);
}

