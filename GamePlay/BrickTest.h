#pragma once
#include "GameObject.h"
#include "TextureManager.h"
#include "Renderer.h"

// Generic static tile: ground, brick, ? block, pipe, used block.
// Which sprite/animation is rendered is controlled by animId passed to constructor.
class Brick : public GameObject
{
	int animId;
public:
	Brick(float x, float y, int animId = 100) : GameObject(x, y)
	{
		this->animId = animId;
		this->vx = 0;
		this->vy = 0;
	}

	int GetAnimId() const { return animId; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	virtual bool IsCollidable() { return true; }
	virtual bool IsBlocking() { return true; }
};