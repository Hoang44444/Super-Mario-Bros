#pragma once
#include "GameObject.h"
#include "TextureManager.h"
#include "Renderer.h"

class Brick : public GameObject
{
private:
	float leftLimit, rightLimit;
	LPTEXTURE texture;
public:
	Brick(float x, float y, float leftLimit, float rightLimit, float vx = 0.1f) : GameObject(x, y) {
		this->leftLimit = leftLimit;
		this->rightLimit = rightLimit;
		this->vx = vx;
		this->vy = 0;
		// Normally texture would be loaded from a manager, 
		// but here we allow it to be set or use a default
		this->texture = NULL; 
	};

	void SetTexture(LPTEXTURE tex) { this->texture = tex; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};

