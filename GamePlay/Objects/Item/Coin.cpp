#include "Coin.h"
#include "Mario.h"
#include "CoinCollectEffect.h"
#include "SoundManager.h"
#include "AssetID.h"
#include "ScoreManager.h"

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
