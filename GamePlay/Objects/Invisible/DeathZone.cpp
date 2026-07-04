#include "DeathZone.h"
#include "Mario.h"

void DeathZone::OnMarioCollision(Mario* mario) {
	mario->SetState(MARIO_STATE::DIE);
}

void DeathZone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	// Không cần update vì vô hình và không di chuyển
}

void DeathZone::Render() {
	// Không cần render vì vô hình
}

void DeathZone::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + DEATHZONE_BBOX::WIDTH;
	b = y + DEATHZONE_BBOX::HEIGHT;
}
