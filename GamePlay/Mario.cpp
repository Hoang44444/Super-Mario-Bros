#include "Mario.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"
#include "Bullet.h"
#include "BrickTest.h"

void Mario::MovementUpdate(DWORD dt) {
	vx += accelX * dt;
	vy += gravity * dt;

	// Friction logic when IDLE
	if (state == MARIO_STATE_IDLE) {
		if (vx > 0) {
			vx -= MARIO_ACCEL_WALK * dt;
			if (vx < 0) vx = 0;
		}
		else if (vx < 0) {
			vx += MARIO_ACCEL_WALK * dt;
			if (vx > 0) vx = 0;
		}
	}
	else if(state == MARIO_STATE_DIE) {
		vx = 0;
	}

	// Max speed capping
	float maxV = MARIO_VELOCITY_WALK_MAX;
	if (state == MARIO_STATE_RUNNING_RIGHT || state == MARIO_STATE_RUNNING_LEFT)
		maxV = MARIO_VELOCITY_RUN_MAX;

	if (abs(vx) > maxV) {
		vx = (vx > 0) ? maxV : -maxV;
	}
}

void Mario::ShootBullet() {
	float bulletX = x + (direction > 0 ? 15.0f : -8.0f);
	float bulletY = y;
	scene->AddObject(new Bullet(bulletX, bulletY, direction, this));
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	MovementUpdate(dt);
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void Mario::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		accelX = MARIO_ACCEL_WALK;
		direction = 1;
		break;

	case MARIO_STATE_WALKING_LEFT:
		accelX = -MARIO_ACCEL_WALK;
		direction = -1;
		break;

	case MARIO_STATE_RUNNING_RIGHT:
		accelX = MARIO_ACCEL_RUN;
		direction = 1;
		break;

	case MARIO_STATE_RUNNING_LEFT:
		accelX = -MARIO_ACCEL_RUN;
		direction = -1;
		break;

	case MARIO_STATE_JUMP:
		if (isOnGround) {
			vy = -MARIO_JUMP_SPEED;
			isOnGround = false;
		}
		break;

	case MARIO_STATE_IDLE:
		accelX = 0;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_SPEED;
		accelX = 0;
		break;

	case MARIO_STATE_SHOOT:
		ShootBullet();
		break;

	case MARIO_STATE_SIT:
		accelX = 0;
		break;
	}
}

void Mario::Render()
{
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
	{
		if (!isOnGround) {
			if (direction > 0) aniId = ID_ANI_MARIO_BIG_JUMP_WALK_RIGHT;
			else aniId = ID_ANI_MARIO_BIG_JUMP_WALK_LEFT;
		}
		else if (vx != 0) {
			if (direction > 0) aniId = ID_ANI_MARIO_BIG_WALKING_RIGHT;
			else aniId = ID_ANI_MARIO_BIG_WALKING_LEFT;
		}
		else {
			if (direction > 0) aniId = ID_ANI_MARIO_BIG_IDLE_RIGHT;
			else aniId = ID_ANI_MARIO_BIG_IDLE_LEFT;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (!isOnGround) {
			if (direction > 0) aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
		else if (vx != 0) {
			if (direction > 0) aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			else aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
		}
		else {
			if (direction > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
			else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
		}
	}

	if (aniId != -1)
		AnimationManager::GetInstance()->Get(aniId)->Render(x, y);
}

void Mario::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	if (level == MARIO_LEVEL_BIG)
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

void Mario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if(dynamic_cast<Brick*>(e->obj))
	{
		if (e->ny < 0 && e->obj->IsBlocking()) { 
			vy = 0;
			isOnGround = true;
		}
	}
}

void Mario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

