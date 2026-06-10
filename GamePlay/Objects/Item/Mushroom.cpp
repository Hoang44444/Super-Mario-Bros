#include "Mushroom.h"
#include "Mario.h"
#include "AnimationManager.h"
#include "AssetID.h"
#include "StaticObject.h"

void Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += MUSHROOM_GRAVITY * dt;
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void Mushroom::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::MUSHROOM)->Render(x, y, z);
}

void Mushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x; t = y;
	r = x + MUSHROOM_BBOX_WIDTH;
	b = y + MUSHROOM_BBOX_HEIGHT;
}

void Mushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Mushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<Mario*>(e->obj))
	{
		OnMarioCollision(dynamic_cast<Mario*>(e->obj));
	}
	else if (dynamic_cast<StaticObject*>(e->obj))
	{
		if (e->ny < 0) vy = 0;
		else if (e->nx != 0) { direction = -direction; vx = MUSHROOM_SPEED * direction; }
	}
}

void Mushroom::OnMarioCollision(Mario* mario)
{
	mario->SetLevel(MARIO_LEVEL::BIG);
	Delete();
}
