#pragma once
#include <vector>
#include "GameObject.h"
#include "AnimationManager.h"

class Mario;

#define ITEM_BBOX_WIDTH 16.0f
#define ITEM_BBOX_HEIGHT 16.0f

constexpr float ITEM_EMERGE_SPEED = 0.03f; // Tốc độ nhảy ra khỏi block (~16px trong ~0.5s)

// Base class cho tất cả các item (mushroom, flower, star, ...)
// Xử lý animation nhảy ra khỏi question block
class Item : public GameObject {
protected:
	int animationId;
	bool emerging = false;       // Đang nhảy ra khỏi question block
	float emergeTargetY = 0;     // Vị trí Y nơi animation nhảy ra kết thúc

	// Tiến hành animation nhảy ra. Trả về true khi vẫn đang nhảy để class con
	// có thể bỏ qua physics cho đến khi item nhảy xong.
	bool UpdateEmerge(DWORD dt) {
		if (!emerging) return false;
		y -= ITEM_EMERGE_SPEED * dt;
		if (y <= emergeTargetY) {
			y = emergeTargetY;
			emerging = false;
			OnEmergeComplete();
		}
		return true;
	}

	// Được gọi 1 lần khi animation nhảy ra kết thúc. Override để bắt đầu
	// chuyển động tự nhiên của item (ví dụ mushroom bắt đầu đi).
	virtual void OnEmergeComplete() {}
public:
	Item(float x, float y, float z, int animationId = -1) : GameObject(x, y, z) {
		this->animationId = animationId;
	}
	virtual ~Item() {}

	// Bắt đầu animation "nhảy ra khỏi block": nhảy lên 1 ô mà không có physics
	virtual void StartEmerge() {
		emerging = true;
		emergeTargetY = y - ITEM_BBOX_HEIGHT;
		vx = 0; vy = 0;
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {
		if (UpdateEmerge(dt)) return;
		Collision::GetInstance()->Process(this, dt, coObjects);
	}
	void Render() {
		if (animationId != -1)
		{
			LPANIMATION ani = AnimationManager::GetInstance()->Get(animationId);
			if (ani != nullptr)
				ani->Render(x, y, z);
		}
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + ITEM_BBOX_WIDTH;
		b = y + ITEM_BBOX_HEIGHT;
	}

	bool IsCollidable() { return true; }
	bool IsBlocking() { return false; }

	// Xử lý khi Mario va chạm với item (mỗi class con định nghĩa riêng)
	virtual void OnMarioCollision(Mario * mario) = 0;
};

