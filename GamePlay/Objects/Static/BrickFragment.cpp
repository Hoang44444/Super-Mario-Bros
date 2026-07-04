#include "BrickFragment.h"
#include "AnimationManager.h"
#include "DeathZone.h"
void BrickFragment::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void BrickFragment::Render() {
	AnimationManager::GetInstance()->Get(aniId)->Render(x, y, z);
}

void BrickFragment::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = t = r = b = 0;
}


void BrickFragment::OnNoCollision(DWORD dt) {
	this->x += this->vx * dt;
	this->y += this->vy * dt;
	this->vy += BRICK_FRAGMENT_PARAMS::GRAVITY * dt; // Hiệu ứng trọng lực
}

void BrickFragment::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (dynamic_cast<DeathZone*>(e->obj)) {
		this->Delete(); // Xóa mảnh nếu chạm death zone
	}
}