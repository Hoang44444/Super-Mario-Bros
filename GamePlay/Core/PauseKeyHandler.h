#pragma once
#include "KeyEventHandler.h"
#include "GameManager.h"
#include "AssetID.h"

class PauseKeyHandler : public KeyEventHandler
{
public:
	void KeyState(BYTE* states) override {}

	void OnKeyDown(int KeyCode) override {
		GameManager* gm = GameManager::GetInstance();
		switch (KeyCode)
		{
		case 'P':
			gm->SetGameState(GAME_STATE::PLAY);
			break;
		case VK_ESCAPE:
			gm->SetGameState(GAME_STATE::PLAY);
			gm->InitiateSwitchScene(SCENE::MENU);
			break;
		}
	}

	void OnKeyUp(int KeyCode) override {}
};
