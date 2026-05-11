#include "OneUpMushroom.h"
#include "AnimationManager.h"
#include "BrickTest.h"
#include "../Resource/AssetID.h"

OneUpMushroom::OneUpMushroom(float x, float y) : Item(x, y)
{
	vx = ONE_UP_SPEED_X;
	vy = 0;
}

void OneUpMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDeleted) return;

	vy += ONE_UP_GRAVITY * dt;
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void OneUpMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void OneUpMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<Brick*>(e->obj))
	{
		if (e->ny != 0) {
			vy = 0; // Chạm đất thì dừng rơi
		}
		else if (e->nx != 0) {
			vx = -vx; // Chạm tường thì đổi hướng
		}
	}
}

void OneUpMushroom::Render()
{
	if (isDeleted) return;
	AnimationManager::GetInstance()->Get(ID_ANI_ONE_UP_MUSHROOM)->Render(x, y);
}