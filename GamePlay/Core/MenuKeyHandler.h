#pragma once
#include "KeyEventHandler.h"
#include "GameManager.h"
#include "PlayerData.h"
#include "PlayScene.h"
#include "MenuOptions.h"
#include "MenuHUD.h"
#include "AssetID.h"

// Keyboard handler for the non-gameplay screens (menu / control / end / death).
// On the MENU it moves the selection cursor over the options and confirms with
// Enter/Space; the option list, count and per-option target scene all come from
// the scene's MenuOptions (loaded from txt). On the CONTROL/END screens any key
// returns to the menu.
class MenuKeyHandler : public KeyEventHandler
{
public:
	MenuKeyHandler() : KeyEventHandler() {}

	void KeyState(BYTE* states) override {}

	void OnKeyDown(int KeyCode) override {
		GameManager* gm = GameManager::GetInstance();
		int sceneId = gm->GetCurrentSceneID();

		if (sceneId == SCENE::MENU) {
			PlayScene* ps = dynamic_cast<PlayScene*>(gm->GetCurrentScene());
			MenuHUD* mh = (ps != nullptr) ? ps->GetMenuHUD() : nullptr;
			if (mh == nullptr) return;

			switch (KeyCode) {
			case VK_UP:
				mh->MoveSelection(-1);
				break;
			case VK_DOWN:
				mh->MoveSelection(+1);
				break;
			case VK_RETURN:
			case VK_SPACE:
				{
					int selected = mh->GetSelectedOption();
					switch (selected) {
					case 0: // START
						PlayerData::Get().Reset();
						PlayerData::Get().returnScene = SCENE::WORLD_1_1;
						gm->InitiateSwitchScene(SCENE::INTRO);
						break;
					case 1: // LEVEL
						gm->InitiateSwitchScene(ID_SCENE_LEVEL);
						break;
					case 2: // HELP
						gm->InitiateSwitchScene(ID_SCENE_HELP);
						break;
					case 3: // QUIT
						// Exit game using existing mechanism
						PostQuitMessage(0);
						break;
					}
				}
				break;
			}
		}
		else if (sceneId == SCENE::DEATH || sceneId == SCENE::INTRO) {
			// Death screen or Intro screen -> press confirm to load/resume the level.
			if (KeyCode == VK_RETURN || KeyCode == VK_SPACE)
				gm->InitiateSwitchScene(PlayerData::Get().returnScene);
		}
		else if (sceneId == SCENE::CONTROL || sceneId == SCENE::END) {
			gm->InitiateSwitchScene(SCENE::MENU);                // any key -> back to menu
		}
	}

	void OnKeyUp(int KeyCode) override {}
};
