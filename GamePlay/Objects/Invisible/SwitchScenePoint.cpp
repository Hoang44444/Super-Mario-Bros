#include "SwitchScenePoint.h"
#include "Mario.h"
#include "GameManager.h"
#include "debug.h"

void SwitchScenePoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Collision::GetInstance()->Process(this, dt, coObjects);
}


void SwitchScenePoint::OnMarioCollision(Mario * mario)
{
	DebugOut(L"[INFO] Mario collided with SwitchScenePoint at (%f, %f)\n", x, y);
	int currentSceneID = GameManager::GetInstance()->GetCurrentSceneID();
	int nextSceneID = (currentSceneID + 1) % 12; 
	GameManager::GetInstance()->InitiateSwitchScene(nextSceneID);
}