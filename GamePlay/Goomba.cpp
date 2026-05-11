#include "Goomba.h"
#include "../Graphic/AnimationManager.h"
#include "../Resource/AssetID.h"
#include "../Resource/debug.h"
#include "Collision.h"

Goomba::Goomba(float x, float y) : GameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	this->die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
}

void Goomba::GetBoundingBox(float& l, float& t, float& r, float& b)
{

	float visualWidth = 16.0f;
	float visualHeight = 16.0f;


	float xOffset = (visualWidth - GOOMBA_BBOX_WIDTH) / 2.0f;
	l = x + xOffset;
	r = l + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
	{
		float yOffsetDie = visualHeight - GOOMBA_BBOX_HEIGHT_DIE;
		t = y + yOffsetDie;
		b = t + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		float yOffset = visualHeight - GOOMBA_BBOX_HEIGHT;
		t = y + yOffset;
		b = t + GOOMBA_BBOX_HEIGHT;
	}
}

void Goomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Goomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<Goomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	if (e->nx != 0)
	{
		if ((e->nx < 0 && vx > 0) || (e->nx > 0 && vx < 0))
		{
			vx = -vx;
			direction = -direction;
		}
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

	GameObject::Update(dt, coObjects);
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void Goomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}

	int drawX = (int)round(x);
	int drawY = (int)round(y);
	AnimationManager::GetInstance()->Get(aniId)->Render(drawX, drawY);

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