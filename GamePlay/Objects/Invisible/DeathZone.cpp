#include "DeathZone.h"
#include "Mario.h"

void DeathZone::OnMarioCollision(Mario* mario) {
	mario->SetState(MARIO_STATE::DIE);
}

void DeathZone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	// No need to update anything since it's invisible and doesn't move
}

void DeathZone::Render() {
	// No need to render anything since it's invisible
}

void DeathZone::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + DEATHZONE_BBOX::WIDTH;
	b = y + DEATHZONE_BBOX::HEIGHT;
}
