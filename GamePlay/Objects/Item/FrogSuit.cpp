#include "FrogSuit.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"

void FrogSuit::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::FROG_SUIT)->Render(x, y, z);
}

void FrogSuit::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x; t = y;
	r = x + FROG_SUIT_BBOX_WIDTH;
	b = y + FROG_SUIT_BBOX_HEIGHT;
}

void FrogSuit::OnMarioCollision(Mario* mario)
{
	mario->SetLevel(MARIO_LEVEL::FROG);
	Delete();
}
