#include "DynamicPlatform.h"
#include "StaticObject.h"
#include "debug.h"
#include "AnimationManager.h"
#include "AssetID.h"

void DynamicPlatform::Movement(DWORD dt) {
	// Lưu vị trí hiện tại trước khi di chuyển (để tính delta cho parenting Mario)
	prevX = x;
	prevY = y;

	if (type == DYNAMIC_PLATFORM_TYPE::HORIZONTAL) {
		x += direction * speed * dt;
		if (x < minBound) {
			x = minBound;
			direction = 1; // Đi phải
		}
		else if (x > maxBound) {
			x = maxBound;
			direction = -1; // Đi trái
		}
	}
	else if (type == DYNAMIC_PLATFORM_TYPE::VERTICAL) {
		y += direction * speed * dt;
		if (y < minBound) {
			y = minBound;
			direction = 1; // Xuống
		}
		else if (y > maxBound) {
			y = maxBound;
			direction = -1; // Lên
		}
	}

	// Đồng bộ tốc độ với hướng (có thể vừa đảo)
	// để các đối tượng khác đọc tốc độ không bị trễ 1 frame
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
	// Chạm vật tĩnh hoặc sàn khác -> đảo hướng
	if (dynamic_cast<StaticObject*>(e->obj) || dynamic_cast<DynamicPlatform*>(e->obj)) {
		direction *= -1;
		vx = GetVx();
		vy = GetVy();
	}
}

void DynamicPlatform::OnNoCollision(DWORD dt) {
	Movement(dt);
}
