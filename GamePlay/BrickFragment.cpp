#include "BrickFragment.h"
#include "Objects/Invisible/DeathZone.h"
void BrickFragment::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void BrickFragment::Render() {

}


void BrickFragment::OnNoCollision(DWORD dt) {
	this->x += this->vx * dt;
	this->y += this->vy * dt;
	this->vy += BRICK_FRAGMENT_PARAMS::GRAVITY * dt; // gravity effect
}

void BrickFragment::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = t = r = b = 0; 
}

void BrickFragment::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (dynamic_cast<DeathZone*>(e->obj)) {
		this->Delete(); // Remove the fragment if it collides with a death zone
	}
}