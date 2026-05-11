#include "Item.h"

Item::Item(float x, float y) : GameObject(x, y)
{
	this->x = x;
	this->y = y;
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Logic update cơ bản 
}

void Item::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + ITEM_BBOX_WIDTH;
	b = t + ITEM_BBOX_HEIGHT;
}