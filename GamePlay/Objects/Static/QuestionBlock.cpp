#include "QuestionBlock.h"
#include "AnimationManager.h"
#include "AssetID.h"
#include "PlayScene.h"
#include "../../CoinPopupEffect.h"
#include "SoundManager.h"
#include "../Core/ScoreManager.h"

void QuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}

void QuestionBlock::Render() {
	int aniId = (state == QUESTION_BLOCK_STATE::ACTIVE) ? ANIMATION::QUESTION_BLOCK : ANIMATION::QUESTION_BLOCK_DELETED;
	AnimationManager::GetInstance()->Get(aniId)->Render(x, y, z);
}

void QuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + QUESTION_BLOCK_BBOX_SIZE;
	b = y + QUESTION_BLOCK_BBOX_SIZE;
}

void QuestionBlock::Break() {
	state = QUESTION_BLOCK_STATE::DELETED;
}

void QuestionBlock::SpawnItem(Mario* mario) {
	PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
	if (playScene == nullptr) return;

	if (itemType == OBJECT::COIN)
	{
		SoundManager::GetInstance()->PlaySFX(SFX::COIN);
		if (mario != nullptr)
		{
			mario->AddCoin();
		}

		playScene->AddObject(new CoinPopupEffect(x, y - QUESTION_BLOCK_BBOX_SIZE, z));
		return;
	}

	LPGAMEOBJECT item = playScene->CreateItem(itemType, x, y, z);
	if (item == nullptr) return;

	if (itemType != OBJECT::ITEM_COIN2)
		SoundManager::GetInstance()->PlaySFX(SFX::POWERUP_APPEARS);

	item->StartEmerge();   // rise out of the block

	playScene->AddObject(item);
}

void QuestionBlock::OnMarioCollision(Mario* mario, LPCOLLISIONEVENT e) {
	// Only react to a hit from below while the block still has its item.
	if (e->ny > 0 && state == QUESTION_BLOCK_STATE::ACTIVE) {
		SpawnItem(mario);
		Break();
	}
}
