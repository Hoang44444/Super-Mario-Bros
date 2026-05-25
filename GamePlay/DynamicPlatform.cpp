#include "DynamicPlatform.h"
#include "StaticObject.h"
#include "debug.h"
void DynamicPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void DynamicPlatform::Render() {
	// Render the platform (placeholder)
	// In a real implementation, you would draw the platform sprite here
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
}
