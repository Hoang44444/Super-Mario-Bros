#include "FireFlower.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
FireFlower::FireFlower(float x, float y) : Item(x, y)
{
	vx = 0;
	vy = 0;
}

void FireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDeleted) return;
	// Hoa đứng yên nên không cần xử lý va chạm hay trọng lực
}

void FireFlower::Render()
{
	if (isDeleted) return;
	AnimationManager::GetInstance()->Get(ID_ANI_FIRE_FLOWER)->Render(x, y);
}