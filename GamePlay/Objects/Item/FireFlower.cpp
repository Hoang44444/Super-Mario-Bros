#include "FireFlower.h"
#include "Mario.h"
#include "AnimationManager.h"
#include "AssetID.h"
#include "SoundManager.h"
#include "../Core/ScoreManager.h"

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
	SoundManager::GetInstance()->PlaySFX(SFX::POWERUP);
	mario->SetLevel(MARIO_LEVEL::FIRE);
	
	// Cộng điểm cho Fire Flower
	ScoreManager::Get().AddScore(SCORE_VALUES::FIRE_FLOWER);
	
	Delete();
}
