#include "HammerSuit.h"
#include "Mario.h"

void HammerSuit::OnMarioCollision(Mario* mario)
{
	mario->SetLevel(MARIO_LEVEL_HAMMER);
	this->Delete();
}