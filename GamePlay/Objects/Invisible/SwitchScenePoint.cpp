#include "SwitchScenePoint.h"
#include "Mario.h"
#include "GameManager.h"
#include "AssetID.h"
#include "debug.h"
#include "PlayScene.h"

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
		nextSceneID = SCENE::END;
	else if (currentSceneID >= SCENE::WORLD_1_1 && currentSceneID < SCENE::WORLD_1_4)
		nextSceneID = currentSceneID + 1;
	else
		nextSceneID = SCENE::MENU;

	PlayerData::Get().returnScene = nextSceneID;

	mario->SetVisible(false);

	PlayScene* playScene = dynamic_cast<PlayScene*>(GameManager::GetInstance()->GetCurrentScene());
	if (playScene != nullptr)
	{
		playScene->StartStageClear();
	}
}