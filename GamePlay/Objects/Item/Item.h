#pragma once
#include <vector>
#include "GameObject.h"
#include "AnimationManager.h"

class Mario;

#define ITEM_BBOX_WIDTH 16.0f
#define ITEM_BBOX_HEIGHT 16.0f

constexpr float ITEM_EMERGE_SPEED = 0.03f;

class Item : public GameObject{
protected:
	int animationId;
	bool emerging = false;
	float emergeTargetY = 0;

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

	virtual void OnEmergeComplete() {}
public:
	Item(float x, float y, float z, int animationId = -1) : GameObject(x, y, z) {
		this->animationId = animationId;
	}
	virtual ~Item() {}

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

	virtual void OnMarioCollision(Mario * mario) = 0;
};

