#include "BowserFireball.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"

BowserFireball::BowserFireball(float x, float y, float z, int dir) : GameObject(x, y, z)
{
	this->direction = dir;
	this->vx = dir * BOWSER_FIREBALL_SPEED;
	this->vy = 0;
}

void BowserFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;

	// Automatically delete if out of bounds (assuming screen width 320)
	if (x < -BOWSER_FIREBALL_WIDTH || x > 320 + BOWSER_FIREBALL_WIDTH)
	{
		this->Delete();
	}
}

void BowserFireball::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::BOWSER_FIRE)->Render(x, y, z);
}

void BowserFireball::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BOWSER_FIREBALL_WIDTH;
	b = y + BOWSER_FIREBALL_HEIGHT;
}