#include "Coin.h"
#include "Mario.h"

void Coin::OnMarioCollision(Mario* mario)
{
	mario->AddCoin();
	this->Delete();
}
