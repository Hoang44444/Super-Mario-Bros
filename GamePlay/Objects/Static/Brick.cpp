#include "Brick.h"
#include "AnimationManager.h"
#include "AssetID.h"
#include "BrickFragment.h"

void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}

void Brick::Render() {
	AnimationManager::GetInstance()->Get(ANIMATION::BRICK)->Render(x, y, z);
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
	// Create 4 fragments
	float fragmentVx[] = { -0.1f, 0.1f, -0.1f, 0.1f };
	float fragmentVy[] = { -0.2f, -0.2f, -0.1f, -0.1f };
	int aniIds[] = { ANIMATION::BRICK_FRAGMENT_UP_LEFT, ANIMATION::BRICK_FRAGMENT_UP_RIGHT, ANIMATION::BRICK_FRAGMENT_BOTTOM_LEFT, ANIMATION::BRICK_FRAGMENT_BOTTOM_RIGHT };
	for (int i = 0; i < 4; i++) {
		float fragmentX = x + (i % 2) * (BRICK_BBOX_WIDTH / 2);
		float fragmentY = y + (i / 2) * (BRICK_BBOX_HEIGHT / 2);
		scene->AddObject(new BrickFragment(fragmentX, fragmentY, this->z, fragmentVx[i], fragmentVy[i], aniIds[i]));
	}
	this->Delete(); // Remove the brick after breaking
}