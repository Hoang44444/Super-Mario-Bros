#include "HammerSuit.h"
#include "Mario.h"
#include "SoundManager.h"

void HammerSuit::OnMarioCollision(Mario* mario)
{
	SoundManager::GetInstance()->PlaySFX(SFX::POWERUP);
	mario->SetLevel(MARIO_LEVEL_HAMMER);
	this->Delete();
}
