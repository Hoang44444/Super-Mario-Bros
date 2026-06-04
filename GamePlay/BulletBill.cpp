#include "BulletBill.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"

BulletBill::BulletBill(float x, float y, float z) : Enemy(x, y, z)
{
	this->direction = 1;
	this->vx = BULLET_BILL_SPEED * direction;
	this->vy = 0;
}

void BulletBill::SetMovement(int dir)
{
	this->direction = dir;
	this->vx = BULLET_BILL_SPEED * dir;
}

void BulletBill::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;

	// Optional: remove if out too far of screen 
	if (x < -BULLET_BILL_WIDTH || x > 1600 + BULLET_BILL_WIDTH)
	{
		this->Delete();
	}
}

void BulletBill::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::BULLET_BILL)->Render(x, y, z);
}

void BulletBill::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BULLET_BILL_WIDTH;
	b = y + BULLET_BILL_HEIGHT;
}

void BulletBill::OnMarioCollison(Mario* mario)
{
	mario->SetState(MARIO_STATE_DIE);
}