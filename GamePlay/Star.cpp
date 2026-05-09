#include "Star.h"
#include "AnimationManager.h"
#include "BrickTest.h" 
#include "../Resource/AssetID.h"
Star::Star(float x, float y) : Item(x, y)
{
	vx = STAR_SPEED_X;
	vy = 0;
}

void Star::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDeleted) return;

	vy += STAR_GRAVITY * dt;
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void Star::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Star::OnCollisionWith(LPCOLLISIONEVENT e)
{
	// Cần có include BrickTest.h hoặc class sàn/tường của bạn
	if (dynamic_cast<Brick*>(e->obj))
	{
		if (e->ny != 0) {
			// Nếu chạm đất (ny < 0) -> Nảy lên
			vy = -STAR_BOUNCE_SPEED;
		}
		else if (e->nx != 0) {
			// Đụng tường -> Quay đầu
			vx = -vx;
		}
	}
}

void Star::Render()
{
	if (isDeleted) return;
	AnimationManager::GetInstance()->Get(ID_ANI_STAR)->Render(x, y);
}