#include "PiranhaPlant.h"
#include "AnimationManager.h"
#include "AssetID.h"

PiranhaPlant::PiranhaPlant(float x, float y, float z) : Enemy(x, y, z)
{
	this->originalY = y;
	this->showY = y;
	this->hideY = y + PIRANHA_PLANT_HEIGHT;
	this->y = hideY;
	this->state = PIRANHA_STATE_HIDING;
	this->timer = GetTickCount();
}

void PiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DWORD now = GetTickCount();
	switch (state)
	{
	case PIRANHA_STATE_HIDING:
		if (now - timer > PIRANHA_DELAY_TIME)
		{
			state = PIRANHA_STATE_RISING;
		}
		break;

	case PIRANHA_STATE_RISING:
		y -= PIRANHA_MOVE_SPEED * dt;
		if (y <= showY)
		{
			y = showY;
			state = PIRANHA_STATE_SHOWING;
			timer = now;
		}
		break;

	case PIRANHA_STATE_SHOWING:
		if (now - timer > PIRANHA_DELAY_TIME)
		{
			state = PIRANHA_STATE_FALLING;
		}
		break;

	case PIRANHA_STATE_FALLING:
		y += PIRANHA_MOVE_SPEED * dt;
		if (y >= hideY)
		{
			y = hideY;
			state = PIRANHA_STATE_HIDING;
			timer = now;
		}
		break;
	}
}

void PiranhaPlant::Render()
{
	if (state != PIRANHA_STATE_HIDING)
	{
		LPANIMATION ani = AnimationManager::GetInstance()->Get(ANIMATION::PIRANHA_PLANT_IDLE);
		if (ani != nullptr)
			ani->Render(x, y, z);
	}
}

void PiranhaPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PIRANHA_PLANT_WIDTH;
	b = y + PIRANHA_PLANT_HEIGHT;
}

void PiranhaPlant::OnMarioCollison(Mario* mario, float ny)
{
	if (state != PIRANHA_STATE_HIDING)
	{
		mario->TakeDamage();
	}
}