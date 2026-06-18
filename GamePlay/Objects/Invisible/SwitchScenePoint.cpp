#include "SwitchScenePoint.h"
#include "Mario.h"
#include "GameManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"

void SwitchScenePoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Collision::GetInstance()->Process(this, dt, coObjects);
}


void SwitchScenePoint::OnMarioCollision(Mario * mario)
{
	DebugOut(L"[INFO] Mario collided with SwitchScenePoint at (%f, %f)\n", x, y);
	int currentSceneID = GameManager::GetInstance()->GetCurrentSceneID();

	int nextSceneID;
	if (currentSceneID == SCENE::WORLD_1_4)
		nextSceneID = SCENE::END;                                       // finished last level -> end scene
	else if (currentSceneID >= SCENE::WORLD_1_1 && currentSceneID < SCENE::WORLD_1_4)
		nextSceneID = currentSceneID + 1;                               // advance to the next level
	else
		nextSceneID = SCENE::MENU;                                      // fallback

	GameManager::GetInstance()->InitiateSwitchScene(nextSceneID);
}