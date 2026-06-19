#pragma once
#include "KeyEventHandler.h"
#include "GameManager.h"
#include "PlayerData.h"
#include "PlayScene.h"
#include "MenuOptions.h"
#include "../Resource/AssetID.h"

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
			MenuOptions* mo = (ps != nullptr) ? ps->GetMenuOptions() : nullptr;
			if (mo == nullptr) return;

			switch (KeyCode) {
			case VK_UP:
				mo->MoveSelection(-1);
				break;
			case VK_DOWN:
				mo->MoveSelection(+1);
				break;
			case VK_RETURN:
			case VK_SPACE:
				gm->InitiateSwitchScene(mo->GetSelectedTarget());   // target defined in txt
				break;
			}
		}
		else if (sceneId == SCENE::DEATH) {
			// Death screen while lives remain -> press confirm to resume the level.
			// (Out-of-lives is GAME_STATE::GAME_OVER, handled by GameOverHandler.)
			if (KeyCode == VK_RETURN || KeyCode == VK_SPACE)
				gm->InitiateSwitchScene(PlayerData::Get().returnScene);
		}
		else if (sceneId == SCENE::CONTROL || sceneId == SCENE::END) {
			gm->InitiateSwitchScene(SCENE::MENU);                // any key -> back to menu
		}
	}

	void OnKeyUp(int KeyCode) override {}
};
