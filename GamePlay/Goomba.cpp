#include "Goomba.h"
#include "../Graphic/AnimationManager.h"
#include "../Resource/AssetID.h"
#include "../Resource/debug.h"

Goomba::Goomba(float x, float y) : GameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	this->die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
}

void Goomba::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
	{
		b = y + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		b = y + GOOMBA_BBOX_HEIGHT;
	}
}

void Goomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Goomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
		direction = -direction;
	}
}

void Goomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	if (state != GOOMBA_STATE_DIE) {
		Collision::GetInstance()->Process(this, dt, coObjects);
	}
}

void Goomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;

	if (state == GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}

	AnimationManager::GetInstance()->Get(aniId)->Render(x, y);
}

void Goomba::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;

	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		direction = -1;
		break;
	}
}