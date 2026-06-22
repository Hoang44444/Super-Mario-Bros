#include "Coin.h"
#include "Mario.h"
#include "CoinCollectEffect.h"
#include "../Resource/SoundManager.h"
#include "../Resource/AssetID.h"

namespace
{
	template <typename T>
	auto AddCoinScore(T* mario, int) -> decltype(mario->AddScore(200), void())
	{
		if (mario != nullptr)
			mario->AddScore(200);
	}

	void AddCoinScore(...)
	{
	}
}

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
	AddCoinScore(mario, 0);

	if (scene != nullptr)
		scene->AddObject(new CoinCollectEffect(x, y, z));

	this->Delete();
}
