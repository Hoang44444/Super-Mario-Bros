#include "Brick.h"
#include "AnimationManager.h"
#include "AssetID.h"
#include "BrickFragment.h"
#include "SoundManager.h"
#include "ScoreManager.h"

void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}

void Brick::Render() {
	AnimationManager::GetInstance()->Get(animationId)->Render(x, y, z);
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void Brick::OnMarioCollision(Mario* mario, LPCOLLISIONEVENT e) {
	if (mario->GetLevel() == MARIO_LEVEL::SMALL) return;
	if (e->ny > 0) {
		Break();
	}
}

void Brick::Break() {
	SoundManager::GetInstance()->PlaySFX(SFX::BRICK_BREAK);

	ScoreManager::Get().AddScore(SCORE_VALUES::BRICK_DESTROYED);

	float fragmentVx[] = { -0.1f, 0.1f, -0.1f, 0.1f };
	float fragmentVy[] = { -0.2f, -0.2f, -0.1f, -0.1f };

	bool isUnderworld = (animationId == ANIMATION::BRICK_UNDERWORLD);
	int aniIds[] = {
		isUnderworld ? ANIMATION::BRICK_FRAGMENT_UNDERWORLD_UP_LEFT     : ANIMATION::BRICK_FRAGMENT_OVERWORLD_UP_LEFT,
		isUnderworld ? ANIMATION::BRICK_FRAGMENT_UNDERWORLD_UP_RIGHT    : ANIMATION::BRICK_FRAGMENT_OVERWORLD_UP_RIGHT,
		isUnderworld ? ANIMATION::BRICK_FRAGMENT_UNDERWORLD_BOTTOM_LEFT : ANIMATION::BRICK_FRAGMENT_OVERWORLD_BOTTOM_LEFT,
		isUnderworld ? ANIMATION::BRICK_FRAGMENT_UNDERWORLD_BOTTOM_RIGHT: ANIMATION::BRICK_FRAGMENT_OVERWORLD_BOTTOM_RIGHT,
	};

	for (int i = 0; i < 4; i++) {
		float fragmentX = x + (i % 2) * (BRICK_BBOX_WIDTH / 2);
		float fragmentY = y + (i / 2) * (BRICK_BBOX_HEIGHT / 2);
		scene->AddObject(new BrickFragment(fragmentX, fragmentY, this->z, fragmentVx[i], fragmentVy[i], aniIds[i]));
	}
	this->Delete();
}
