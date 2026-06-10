#include "FireFlower.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"

void FireFlower::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::FIRE_FLOWER)->Render(x, y, z);
}

void FireFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x; t = y;
	r = x + FIRE_FLOWER_BBOX_WIDTH;
	b = y + FIRE_FLOWER_BBOX_HEIGHT;
}

void FireFlower::OnMarioCollision(Mario* mario)
{
	mario->SetLevel(MARIO_LEVEL::FIRE);
	Delete();
}
