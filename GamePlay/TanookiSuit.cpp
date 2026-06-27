#include "TanookiSuit.h"
#include "Mario.h"
#include "SoundManager.h"

void TanookiSuit::OnMarioCollision(Mario* mario)
{
	SoundManager::GetInstance()->PlaySFX(SFX::POWERUP);
	mario->SetLevel(MARIO_LEVEL_TANOOKI);
	this->Delete();
}
