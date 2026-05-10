#pragma once
#include "GameObject.h"

#define ITEM_BBOX_WIDTH		16
#define ITEM_BBOX_HEIGHT	16

class Item : public GameObject
{
public:
	Item(float x, float y);
	virtual ~Item() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override = 0; // Kế thừa ảo xuống các con
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	virtual bool IsCollidable() override { return true; }
	virtual bool IsBlocking() override { return false; }
};