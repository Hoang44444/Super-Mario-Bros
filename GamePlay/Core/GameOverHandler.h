#pragma once
#include "KeyEventHandler.h"
#include "GameManager.h"
#include "PlayerData.h"
#include "../Resource/AssetID.h"

// Xử lý input khi màn hình Game Over xuất hiện (GAME_STATE::GAME_OVER, hết mạng)
// Enter/Space reset dữ liệu người chơi và về menu chính
class GameOverHandler : public KeyEventHandler
{
public:
	void KeyState(BYTE* states) override {}

	void OnKeyDown(int KeyCode) override {
		if (KeyCode == VK_SPACE)
		{
			PlayerData::Get().Reset();
			GameManager::GetInstance()->InitiateSwitchScene(SCENE::MENU);
		}
	}

	void OnKeyUp(int KeyCode) override {}
};
