#include "FrogSuit.h"
#include "Mario.h"
#include "AnimationManager.h"
#include "AssetID.h"
#include "SoundManager.h"

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
	SoundManager::GetInstance()->PlaySFX(SFX::POWERUP);
	mario->SetLevel(MARIO_LEVEL::FROG);
	Delete();
}
