#include "Coin.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
/*
#ifndef ID_ANI_COIN
#define ID_ANI_COIN 100
#endif
*/
Coin::Coin(float x, float y) : Item(x, y)
{
	vx = 0;
	vy = 0; // Không bị rơi
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDeleted) return;
	// Coin đứng tĩnh nên không cần tính toán tọa độ
}

void Coin::Render()
{
	if (isDeleted) return;
	AnimationManager::GetInstance()->Get(ID_ANI_COIN)->Render(x, y);
}