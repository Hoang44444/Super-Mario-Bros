#pragma once
#include "KeyEventHandler.h"
#include "GameManager.h"
#include "../Resource/AssetID.h"

// Input handler used while the game is paused (GAME_STATE::PAUSE).
//   'P'  : resume play
//   Esc  : quit to the menu
class PauseKeyHandler : public KeyEventHandler
{
public:
	void KeyState(BYTE* states) override {}

	void OnKeyDown(int KeyCode) override {
		GameManager* gm = GameManager::GetInstance();
		switch (KeyCode)
		{
		case 'P':
			gm->SetGameState(GAME_STATE::PLAY);     // resume
			break;
		case VK_ESCAPE:
			gm->SetGameState(GAME_STATE::PLAY);     // leave pause so the switch can run
			gm->InitiateSwitchScene(SCENE::MENU);   // quit to menu
			break;
		}
	}

	void OnKeyUp(int KeyCode) override {}
};
