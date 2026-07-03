#include "Coin.h"
#include "Mario.h"
#include "CoinCollectEffect.h"
#include "../Resource/SoundManager.h"
#include "../Resource/AssetID.h"
#include "Core/ScoreManager.h"

void Coin::Render()
{
	if (collected || IsDeleted()) return;
	Item::Render();
}

void Coin::OnMarioCollision(Mario* mario)
{
	if (collected || IsDeleted() || mario == nullptr) return;

	collected = true;

	SoundManager::GetInstance()->PlaySFX(SFX::COIN);
	mario->AddCoin();

	if (scene != nullptr)
		scene->AddObject(new CoinCollectEffect(x, y, z));

	this->Delete();
}
