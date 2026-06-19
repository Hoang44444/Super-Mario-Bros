#pragma once
#include "KeyEventHandler.h"
#include "PlayScene.h"
#include "Mario.h"
#include "GameManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"

class MarioKeyHandler : public KeyEventHandler
{
	PlayScene* scene;

public:
	MarioKeyHandler(PlayScene* s) : KeyEventHandler() {
		scene = s;
	}

	virtual void KeyState(BYTE* states) {
		LPGAMEOBJECT mario = scene->GetPlayer();
		if (mario == NULL) return;

		if (mario->GetState() == MARIO_STATE::DIE) return;

		if (states[VK_RIGHT] & 0x80)
			mario->SetState(MARIO_STATE::WALKING_RIGHT);
		else if (states[VK_LEFT] & 0x80)
			mario->SetState(MARIO_STATE::WALKING_LEFT);
		else
			mario->SetState(MARIO_STATE::IDLE);
	}

	virtual void OnKeyDown(int KeyCode) {
		// 'P' pauses the game. While paused, input is handled by PauseKeyHandler
		// (GameManager routes by game state), so we only need to enter PAUSE here.
		if (KeyCode == 'P') {
			GameManager::GetInstance()->SetGameState(GAME_STATE::PAUSE);
			return;
		}

		LPGAMEOBJECT mario = scene->GetPlayer();
		if (mario == NULL) return;

		if (mario->GetState() == MARIO_STATE::DIE) return;

		switch (KeyCode)
		{
		case VK_SPACE:
			mario->SetState(MARIO_STATE::JUMP);
			break;
		case VK_SHIFT:
			DebugOut(L"[KEYBOARD] Key 'Shift' pressed: Shooting bullet\n");
			mario->SetState(MARIO_STATE::SHOOT);
			break;
		}
	}

	virtual void OnKeyUp(int KeyCode) {
		// Handle key release if needed
	}
};
