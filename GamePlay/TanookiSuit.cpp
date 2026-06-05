#include "TanookiSuit.h"
#include "Mario.h"

void TanookiSuit::OnMarioCollision(Mario* mario)
{
	mario->SetLevel(MARIO_LEVEL_TANOOKI);
	this->Delete();
}