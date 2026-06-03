#include "PoisonMushroom.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "StaticObject.h"

void PoisonMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += POISON_MUSHROOM_GRAVITY * dt;
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void PoisonMushroom::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::POISON_MUSHROOM)->Render(x, y, z);
}

void PoisonMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + POISON_MUSHROOM_BBOX_WIDTH;
	b = y + POISON_MUSHROOM_BBOX_HEIGHT;
}

void PoisonMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void PoisonMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<Mario*>(e->obj))
	{
		OnMarioCollision(dynamic_cast<Mario*>(e->obj));
	}
	else if (dynamic_cast<StaticObject*>(e->obj))
	{
		if (e->ny < 0)
		{
			vy = 0;
		}
		else if (e->nx != 0)
		{
			direction = -direction;
			vx = POISON_MUSHROOM_SPEED * direction;
		}
	}
}

void PoisonMushroom::OnMarioCollision(Mario* mario)
{
	if (mario->IsInvincible())
	{
		Delete();
		return;
	}

	if (mario->CanShoot())
	{
		mario->SetCanShoot(false);
	}
	if (mario->GetLevel() != MARIO_LEVEL_SMALL)
	{
		mario->SetLevel(MARIO_LEVEL_SMALL);
	}
	else
	{
		mario->SetState(MARIO_STATE_DIE);
	}

	Delete();
}
