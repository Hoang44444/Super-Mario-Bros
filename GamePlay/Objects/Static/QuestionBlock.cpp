#include "QuestionBlock.h"
#include "AnimationManager.h"
#include "AssetID.h"

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

void QuestionBlock::OnMarioCollision(Mario* mario, LPCOLLISIONEVENT e) {
	if (e->ny > 0) {
		Break();
	}
}
