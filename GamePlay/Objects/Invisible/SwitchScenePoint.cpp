#include "SwitchScenePoint.h"
#include "Mario.h"
#include "GameManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"
#include "../Scenes/PlayScene.h"

void SwitchScenePoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Collision::GetInstance()->Process(this, dt, coObjects);
}


void SwitchScenePoint::OnMarioCollision(Mario * mario)
{
	int currentSceneID = GameManager::GetInstance()->GetCurrentSceneID();

	int nextSceneID;
	if (currentSceneID == SCENE::WORLD_1_4)
		nextSceneID = SCENE::END;                                       // finished last level -> end scene
	else if (currentSceneID >= SCENE::WORLD_1_1 && currentSceneID < SCENE::WORLD_1_4)
		nextSceneID = currentSceneID + 1;                               // advance to the next level
	else
		nextSceneID = SCENE::MENU;                                      // fallback

	// Set returnScene to the next level
	PlayerData::Get().returnScene = nextSceneID;

	// Hide Mario (make invisible) but keep him alive for input during stage clear
	mario->SetVisible(false);

	// Start stage clear sequence instead of immediately switching
	PlayScene* playScene = dynamic_cast<PlayScene*>(GameManager::GetInstance()->GetCurrentScene());
	if (playScene != nullptr)
	{
		playScene->StartStageClear();
	}
}