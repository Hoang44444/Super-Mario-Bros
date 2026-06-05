#pragma once
#include <vector>
#include "GameObject.h"
#include "AnimationManager.h"

class Mario;

#define ITEM_BBOX_WIDTH 16.0f
#define ITEM_BBOX_HEIGHT 16.0f

class Item : public GameObject{
protected:
	int animationId;
public: 
	Item(float x, float y, float z, int animationId = -1) : GameObject(x, y, z) {
		this->animationId = animationId;
	}
	Item(float x, float y, float z) : GameObject(x, y, z) {}
	virtual ~Item() {}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {
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

