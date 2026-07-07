#include "Hammer.h"
#include "AnimationManager.h"
#include "AssetID.h"
#include "PlayScene.h"
#include "Mario.h"

Hammer::Hammer(float x, float y, float z, int dir) : GameObject(x, y, z)
{
	this->direction = dir;
	this->vx = dir * HAMMER_SPEED_X;
	this->vy = -HAMMER_SPEED_Y;
}

void Hammer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += HAMMER_GRAVITY * dt;
	x += vx * dt;
	y += vy * dt;

	float hl, ht, hr, hb;
	GetBoundingBox(hl, ht, hr, hb);

	PlayScene* ps = dynamic_cast<PlayScene*>(scene);
	if (ps != nullptr)
	{
		Mario* mario = dynamic_cast<Mario*>(ps->GetPlayer());
		if (mario != nullptr && mario->GetState() != MARIO_STATE::DIE)
		{
			float ml, mt, mr, mb;
			mario->GetBoundingBox(ml, mt, mr, mb);
			if (hl < mr && hr > ml && ht < mb && hb > mt)
			{
				mario->TakeDamage();
				this->Delete();
				return;
			}
		}
	}

	if (coObjects != nullptr)
	{
		for (auto o : *coObjects)
		{
			if (o == this || o->IsDeleted() || !o->IsBlocking()) continue;
			float ol, ot, orr, ob;
			o->GetBoundingBox(ol, ot, orr, ob);
			if (hl < orr && hr > ol && ht < ob && hb > ot)
			{
				this->Delete();
				return;
			}
		}
	}

	if (y > 400)
	{
		this->Delete();
	}
}

void Hammer::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::HAMMER)->Render(x, y, z);
}

void Hammer::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + HAMMER_WIDTH;
	b = y + HAMMER_HEIGHT;
}