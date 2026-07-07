#pragma once
#include "KeyEventHandler.h"
#include "GameManager.h"
#include "PlayerData.h"
#include "AssetID.h"

// Input handler used on the game-over screen (GAME_STATE::GAME_OVER, out of lives).
// Enter/Space resets the run and replays from the first level ("chÆ¡i láº¡i tá»« Ä‘áº§u").
class GameOverHandler : public KeyEventHandler
{
public:
	void KeyState(BYTE* states) override {}

	void OnKeyDown(int KeyCode) override {
		if (KeyCode == VK_RETURN || KeyCode == VK_SPACE)
		{
			PlayerData::Get().Reset();
			GameManager::GetInstance()->InitiateSwitchScene(SCENE::MENU);
		}
	}

	void OnKeyUp(int KeyCode) override {}
};
