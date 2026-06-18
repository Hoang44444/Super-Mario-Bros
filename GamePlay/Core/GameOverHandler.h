#pragma once
#include "KeyEventHandler.h"
#include "GameManager.h"
#include "PlayerData.h"
#include "../Resource/AssetID.h"

// Input handler used on the game-over screen (GAME_STATE::GAME_OVER, out of lives).
// A confirm key resets the run and returns to the menu.
class GameOverHandler : public KeyEventHandler
{
public:
	void KeyState(BYTE* states) override {}

	void OnKeyDown(int KeyCode) override {
		if (KeyCode == VK_RETURN || KeyCode == VK_SPACE)
		{
			PlayerData::Get().Reset();                                  // fresh lives for a new run
			GameManager::GetInstance()->InitiateSwitchScene(SCENE::MENU);
		}
	}

	void OnKeyUp(int KeyCode) override {}
};
