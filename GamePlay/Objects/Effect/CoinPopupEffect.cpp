#include "CoinPopupEffect.h"

#include "AnimationManager.h"
#include "AssetID.h"

CoinPopupEffect::CoinPopupEffect(float x, float y, float z) : GameObject(x, y, z)
{
	elapsedTime = 0;
	vy = RISE_SPEED;
}

void CoinPopupEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	elapsedTime += dt;

	if (elapsedTime >= LIFETIME)
		Delete();
}

void CoinPopupEffect::Render()
{
	LPANIMATION ani = AnimationManager::GetInstance()->Get(ANIMATION::ITEM_COIN);
	if (ani != nullptr)
		ani->Render(x, y, z);
}

void CoinPopupEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}
