#pragma once
#include "KeyEventHandler.h"
#include "GameManager.h"
#include "../Resource/AssetID.h"

// Xử lý phím khi game đang pause (GAME_STATE::PAUSE)
// P: Tiếp tục chơi
// Esc: Về menu chính
class PauseKeyHandler : public KeyEventHandler
{
public:
	void KeyState(BYTE* states) override {}

	void OnKeyDown(int KeyCode) override {
		GameManager* gm = GameManager::GetInstance();
		switch (KeyCode)
		{
		case 'P':
			gm->SetGameState(GAME_STATE::PLAY);  // Tiếp tục chơi
			break;
		case VK_ESCAPE:
			gm->SetGameState(GAME_STATE::PLAY);  // Thoát pause để chuyển scene
			gm->InitiateSwitchScene(SCENE::MENU);  // Về menu
			break;
		}
	}

	void OnKeyUp(int KeyCode) override {}
};
