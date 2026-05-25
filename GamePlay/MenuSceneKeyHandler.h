#pragma once
#include "KeyEventHandler.h"
#include "GameManager.h"
#include "MenuScene.h"
#include "debug.h"

class MenuSceneKeyHandler : public KeyEventHandler
{
private:
    MenuScene* menuScene;
public:
    MenuSceneKeyHandler(MenuScene* menuScene) : KeyEventHandler() {
        this->menuScene = menuScene;
    }

    virtual void KeyState(BYTE *states) {}
    virtual void OnKeyDown(int KeyCode) {
        if (KeyCode == VK_RETURN) {
            GameManager::GetInstance()->InitiateSwitchScene(2); // Switch to scene_test
        }
    }
    virtual void OnKeyUp(int KeyCode) {}
};
