#include "SuperLeaf.h"
#include "Mario.h"

void SuperLeaf::OnMarioCollision(Mario* mario)
{
	mario->SetLevel(MARIO_LEVEL_RACCOON);
	this->Delete();
}