#pragma once
#include "GameObject.h"

// Invisible trigger zone.
// When Mario's bounding box overlaps this, PlayScene switches to scene_id.
// Scene txt format:  50  x  y  width  height  scene_id
class Portal : public GameObject
{
	float width, height;
	int scene_id;

public:
	Portal(float x, float y, float width, float height, int scene_id)
		: GameObject(x, y)
	{
		this->width = width;
		this->height = height;
		this->scene_id = scene_id;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override {}
	virtual void Render() override {}

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override
	{
		l = x;         t = y;
		r = x + width; b = y + height;
	}

	virtual bool IsCollidable() { return false; }
	virtual bool IsBlocking() { return false; }

	int GetSceneId() { return scene_id; }
};