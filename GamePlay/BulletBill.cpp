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
	// xóa nếu ra quá xa màn hình 
	if (x < 0 || x > 5120)
	{
		Delete();
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

void BulletBill::OnMarioCollison(Mario* mario, float ny)
{
	mario->TakeDamage();
}