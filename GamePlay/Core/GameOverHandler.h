#pragma once
#include "KeyEventHandler.h"
#include "GameManager.h"
#include "PlayerData.h"
#include "AssetID.h"

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
