#include "CastleBridge.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"

void CastleBridge::Collapse(DWORD delay)
{
	isCollapsing = true;
	collapseDelay = delay;
}

void CastleBridge::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isCollapsing && !isFalling)
	{
		if (collapseDelay > dt) { collapseDelay -= dt; return; }  // Chờ delay
		collapseDelay = 0;
		isFalling = true;  // Bắt đầu rơi
		vy = 0;
	}

	if (isFalling)
	{
		vy += CASTLE_BRIDGE_GRAVITY * dt;  // Trọng lực
		y  += vy * dt;
		if (y > 260) Delete();  // Xóa khi rơi quá xa
	}
}

void CastleBridge::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::CASTLE_BRIDGE)->Render(x, y, z);
}

void CastleBridge::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x; t = y;
	r = x + CASTLE_BRIDGE_BBOX_WIDTH;
	b = y + CASTLE_BRIDGE_BBOX_HEIGHT;
}
