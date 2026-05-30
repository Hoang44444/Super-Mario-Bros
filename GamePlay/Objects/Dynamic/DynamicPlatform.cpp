#include "DynamicPlatform.h"
#include "StaticObject.h"
#include "debug.h"
#include "AnimationManager.h"
#include "AssetID.h"

void DynamicPlatform::Movement(DWORD dt) {
	if (type == DYNAMIC_PLATFORM_TYPE::HORIZONTAL) {
		x += direction * speed * dt;
		if (x < minBound) {
			x = minBound;
			direction = 1; // Move right
		}
		else if (x > maxBound) {
			x = maxBound;
			direction = -1; // Move left
		}
	}
	else if (type == DYNAMIC_PLATFORM_TYPE::VERTICAL) {
		y += direction * speed * dt;
		if (y < minBound) {
			y = minBound;
			direction = 1; // Move down
		}
		else if (y > maxBound) {
			y = maxBound;
			direction = -1; // Move up
		}
	}

	// keep velocity in sync with the (possibly just-flipped) direction
	// so other objects reading our speed don't lag one frame behind
	this->vx = GetVx();
	this->vy = GetVy();
}

void DynamicPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void DynamicPlatform::Render() {
	AnimationManager::GetInstance()->Get(ANIMATION::DYNAMIC_PLATFORM)->Render(x, y, z);
}

void DynamicPlatform::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + DYNAMIC_PLATFORM_BBOX::WIDTH;
	b = y + DYNAMIC_PLATFORM_BBOX::HEIGHT;
}

void DynamicPlatform::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (dynamic_cast<StaticObject*>(e->obj)) {
		direction *= -1; // Reverse direction on collision with static objects
	}
}

void DynamicPlatform::OnNoCollision(DWORD dt) {
	Movement(dt);
}
