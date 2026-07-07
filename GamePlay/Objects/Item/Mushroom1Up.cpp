#include "Mushroom1Up.h"
#include "Collision.h"
#include "StaticObject.h"
#include "Mario.h"
#include "SoundManager.h"

void Mushroom1Up::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == MUSHROOM_1UP_STATE_SPAWNING)
	{
		y += vy * dt;
		if (start_y - y >= 16.0f)
		{
			state = MUSHROOM_1UP_STATE_WALKING;
			vy = 0;
			vx = MUSHROOM_SPEED;
		}
		return;
	}
	else if (state == MUSHROOM_1UP_STATE_WALKING)
	{
		vy += MUSHROOM_GRAVITY * dt;
	}

	Collision::GetInstance()->Process(this, dt, coObjects);
}

void Mushroom1Up::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Mushroom1Up::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		if (dynamic_cast<StaticObject*>(e->obj))
		{
			vx = -vx;
		}
	}
}

void Mushroom1Up::OnMarioCollision(Mario* mario)
{
    SoundManager::GetInstance()->PlaySFX(SFX::ONE_UP);
    mario->AddLife();
    this->Delete();
}
