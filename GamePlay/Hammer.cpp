#include "Hammer.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"

Hammer::Hammer(float x, float y, float z, int dir) : GameObject(x, y, z)
{
	this->direction = dir;
	this->vx = dir * HAMMER_SPEED_X;
	this->vy = -HAMMER_SPEED_Y; // Initial upward launch
}

void Hammer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Physics: parabolic motion
	vy += HAMMER_GRAVITY * dt;
	x += vx * dt;
	y += vy * dt;

	// Automatically delete if falling too deep (e.g. y > 400)
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