#include "Mushroom.h"
#include "AnimationManager.h"
#include "BrickTest.h"
#include "../Resource/AssetID.h"

Mushroom::Mushroom(float x, float y) : Item(x, y)
{
	vx = MUSHROOM_SPEED_X;
	vy = 0;
}

void Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDeleted) return;

	vy += MUSHROOM_GRAVITY * dt;

	// Kích hoạt bộ xử lý va chạm của framework thay vì tự cộng x, y
	Collision::GetInstance()->Process(this, dt, coObjects);
}

// Nếu không đụng ai, cứ tiếp tục rớt 
void Mushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

// Xử lý khi đụng sàn hoặc tường
void Mushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<Brick*>(e->obj))
	{
		if (e->ny != 0) {
			// Trục Y != 0 nghĩa là chạm từ trên xuống (sàn) hoặc dưới lên
			vy = 0;
		}
		else if (e->nx != 0) {
			// Trục X != 0 nghĩa là đụng tường ngang -> Nấm quay đầu
			vx = -vx;
		}
	}
}

void Mushroom::Render()
{
	if (isDeleted) return;
	LPANIMATION ani = AnimationManager::GetInstance()->Get(ID_ANI_MUSHROOM);

	if (ani != NULL) {
		ani->Render(x, y);
	}
}