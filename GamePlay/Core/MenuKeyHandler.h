#pragma once
#include "KeyEventHandler.h"
#include "GameManager.h"
#include "PlayerData.h"
#include "../Resource/AssetID.h"

// Keyboard handler for the non-gameplay screens (menu / control / end).
// On the MENU it moves a selection cursor between the two options
// (0 = "Bắt đầu" -> start game, 1 = "Hướng dẫn" -> controls) and confirms with
// Enter/Space. On the CONTROL/END screens any key returns to the menu.
class MenuKeyHandler : public KeyEventHandler
{
	int selected = 0;                       // 0 = Bắt đầu, 1 = Hướng dẫn
	static const int OPTION_COUNT = 2;

public:
	MenuKeyHandler() : KeyEventHandler() {}

	int GetSelected() { return selected; }

	void KeyState(BYTE* states) override {}

	void OnKeyDown(int KeyCode) override {
		GameManager* gm = GameManager::GetInstance();
		int sceneId = gm->GetCurrentSceneID();

		if (sceneId == SCENE::MENU) {
			switch (KeyCode) {
			case VK_UP:
				selected = (selected + OPTION_COUNT - 1) % OPTION_COUNT;
				break;
			case VK_DOWN:
				selected = (selected + 1) % OPTION_COUNT;
				break;
			case VK_RETURN:
			case VK_SPACE:
				if (selected == 0)
					gm->InitiateSwitchScene(SCENE::WORLD_1_1);   // Bắt đầu -> play
				else
					gm->InitiateSwitchScene(SCENE::CONTROL);     // Hướng dẫn -> controls
				break;
			}
		}
		else if (sceneId == SCENE::DEATH) {
			// "Press ENTER to continue"
			if (KeyCode == VK_RETURN || KeyCode == VK_SPACE) {
				PlayerData& pd = PlayerData::Get();
				if (pd.lives > 0) {
					gm->InitiateSwitchScene(pd.returnScene);   // still alive -> resume the level
				}
				else {
					pd.Reset();                                // game over -> restart from menu
					gm->InitiateSwitchScene(SCENE::MENU);
				}
			}
		}
		else if (sceneId == SCENE::CONTROL || sceneId == SCENE::END) {
			gm->InitiateSwitchScene(SCENE::MENU);                // any key -> back to menu
		}
	}

	void OnKeyUp(int KeyCode) override {}
};
