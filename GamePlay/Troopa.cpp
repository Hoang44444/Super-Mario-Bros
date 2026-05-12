#include "Troopa.h"
#include "../Graphic/AnimationManager.h"
#include "../Resource/AssetID.h"
#include "Collision.h"

Troopa::Troopa(float x, float y) : GameObject(x, y)
{
	this->ax = 0;
	this->ay = TROOPA_GRAVITY;
	this->die_start = -1;
	SetState(TROOPA_STATE_WALKING);
}

void Troopa::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	r = l + TROOPA_BBOX_WIDTH;

	float FOOT_PADDING = 4.0f;

	float visualBottom = y + 34.0f;

	if (state == TROOPA_STATE_WALKING)
	{
		b = visualBottom - FOOT_PADDING;
		t = b - TROOPA_BBOX_HEIGHT;
	}
	else
	{
		b = visualBottom - FOOT_PADDING;
		t = b - TROOPA_BBOX_HEIGHT_SHELL;
	}
}

void Troopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Troopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == TROOPA_STATE_DIE) return;

	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{

		if ((e->ny < 0 && vy > 0) || (e->ny > 0 && vy < 0))
		{
			vy = 0;
		}
	}
	else if (e->nx != 0)
	{

		if ((e->nx < 0 && vx > 0) || (e->nx > 0 && vx < 0))
		{
			vx = -vx;
		}
	}
}

void Troopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (state == TROOPA_STATE_DIE && (GetTickCount64() - die_start > 1000))
	{
		isDeleted = true;
		return;
	}

	GameObject::Update(dt, coObjects);
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void Troopa::Render()
{
	int aniId = ID_ANI_TROOPA_WALKING_LEFT;

	switch (state)
	{
	case TROOPA_STATE_WALKING:
		aniId = (vx > 0) ? ID_ANI_TROOPA_WALKING_RIGHT : ID_ANI_TROOPA_WALKING_LEFT;
		break;
	case TROOPA_STATE_SHELL:
		aniId = ID_ANI_TROOPA_SHELL;
		break;
	case TROOPA_STATE_SHELL_MOVING:
		aniId = ID_ANI_TROOPA_SHELL_MOVING;
		break;
	case TROOPA_STATE_DIE:
		aniId = ID_ANI_TROOPA_SHELL;
		break;
	}

	AnimationManager::GetInstance()->Get(aniId)->Render(x, y);
}

void Troopa::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case TROOPA_STATE_WALKING:
		vx = -TROOPA_WALKING_SPEED;
		ay = TROOPA_GRAVITY;
		break;

	case TROOPA_STATE_SHELL:
		vx = 0;
		break;

	case TROOPA_STATE_SHELL_MOVING:
		if (vx == 0) vx = TROOPA_SHELL_SPEED;
		break;

	case TROOPA_STATE_DIE:
		die_start = GetTickCount64();
		vy = -0.2f;
		vx = 0;
		ax = 0;
		break;
	}
}