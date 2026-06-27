#include "SuperLeaf.h"
#include "Mario.h"
#include "SoundManager.h"

void SuperLeaf::OnMarioCollision(Mario* mario)
{
	SoundManager::GetInstance()->PlaySFX(SFX::POWERUP);
	mario->SetLevel(MARIO_LEVEL_RACCOON);
	this->Delete();
}
