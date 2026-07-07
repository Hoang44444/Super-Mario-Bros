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
		nextSceneID = SCENE::END;                                       // Level cuối -> màn hình end
	else if (currentSceneID >= SCENE::WORLD_1_1 && currentSceneID < SCENE::WORLD_1_4)
		nextSceneID = currentSceneID + 1;                               // Chuyển sang level tiếp theo
	else
		nextSceneID = SCENE::MENU;                                      // Fallback

	// Set returnScene là level tiếp theo
	PlayerData::Get().returnScene = nextSceneID;

	// Ẩn Mario nhưng giữ sống để input trong stage clear
	mario->SetVisible(false);

	// Bắt đầu sequence stage clear thay vì chuyển ngay lập tức
	PlayScene* playScene = dynamic_cast<PlayScene*>(GameManager::GetInstance()->GetCurrentScene());
	if (playScene != nullptr)
	{
		playScene->StartStageClear();
	}
}