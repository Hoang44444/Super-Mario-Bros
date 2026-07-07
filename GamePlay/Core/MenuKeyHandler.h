#pragma once
#include "KeyEventHandler.h"
#include "GameManager.h"
#include "PlayerData.h"
#include "PlayScene.h"
#include "MenuOptions.h"
#include "../Scenes/MenuHUD.h"
#include "../Resource/AssetID.h"

// Xử lý phím cho các màn hình không phải gameplay (menu/control/end/death)
// MENU: Lên/Xuống di chuyển con trỏ, Space xác nhận
// DEATH/INTRO: Space để tiếp tục chơi
// CONTROL/END: Phím bất kỳ để về menu
class MenuKeyHandler : public KeyEventHandler
{
public:
	MenuKeyHandler() : KeyEventHandler() {}

	void KeyState(BYTE* states) override {}

	void OnKeyDown(int KeyCode) override {
		GameManager* gm = GameManager::GetInstance();
		int sceneId = gm->GetCurrentSceneID();

		// Xử lý MENU chính
		if (sceneId == SCENE::MENU) {
			PlayScene* ps = dynamic_cast<PlayScene*>(gm->GetCurrentScene());
			MenuHUD* mh = (ps != nullptr) ? ps->GetMenuHUD() : nullptr;
			if (mh == nullptr) return;

			switch (KeyCode) {
			case VK_UP:
				mh->MoveSelection(-1);  // Lên
				break;
			case VK_DOWN:
				mh->MoveSelection(+1);  // Xuống
				break;
			case VK_SPACE:
				{
					int selected = mh->GetSelectedOption();
					switch (selected) {
					case 0: // START - Bắt đầu game mới
						PlayerData::Get().Reset();
						PlayerData::Get().returnScene = SCENE::WORLD_1_1;
						gm->InitiateSwitchScene(SCENE::INTRO);
						break;
					case 1: // LEVEL - Chọn level
						gm->InitiateSwitchScene(ID_SCENE_LEVEL);
						break;
					case 2: // HELP - Xem hướng dẫn
						gm->InitiateSwitchScene(ID_SCENE_HELP);
						break;
					case 3: // QUIT - Thoát game
						PostQuitMessage(0);
						break;
					}
				}
				break;
			}
		}
		// Màn hình chết hoặc intro
		else if (sceneId == SCENE::DEATH || sceneId == SCENE::INTRO) {
			if (KeyCode == VK_SPACE)
				gm->InitiateSwitchScene(PlayerData::Get().returnScene);  // Tiếp tục chơi
		}
		// Màn hình control hoặc end
		else if (sceneId == SCENE::CONTROL || sceneId == SCENE::END) {
			gm->InitiateSwitchScene(SCENE::MENU);  // Về menu
		}
	}

	void OnKeyUp(int KeyCode) override {}
};
