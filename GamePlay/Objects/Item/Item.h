#pragma once
#include <vector>
#include "GameObject.h"
#include "AnimationManager.h"

class Mario;

#define ITEM_BBOX_WIDTH 16.0f
#define ITEM_BBOX_HEIGHT 16.0f

constexpr float ITEM_EMERGE_SPEED = 0.03f; // px/ms while rising out of a block (~16px in ~0.5s)

class Item : public GameObject{
protected:
	int animationId;
	bool emerging = false;       // true while rising out of a question block
	float emergeTargetY = 0;     // y position where the emerge animation stops

	// Advance the emerge animation. Returns true while still emerging so derived
	// Update() methods can skip their normal physics until the item has fully risen.
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

	// Called once when the emerge animation finishes. Override to kick off the
	// item's natural motion (e.g. a mushroom starts walking).
	virtual void OnEmergeComplete() {}
public:
	Item(float x, float y, float z, int animationId = -1) : GameObject(x, y, z) {
		this->animationId = animationId;
	}
	virtual ~Item() {}

	// Begin the "rise out of a block" animation: lift one tile upward with no physics.
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

	virtual void OnMarioCollision(Mario * mario) = 0; // Define this in derived classes to specify what happens when Mario collides with the item
};

