#pragma once
#include "KeyEventHandler.h"
#include "PlayScene.h"
#include "Mario.h"
#include "GameManager.h"
#include "AssetID.h"
#include "debug.h"

class MarioKeyHandler : public KeyEventHandler
{
	PlayScene* scene;

	bool jumpPending = false;

	bool flyMode = false;

public:
	MarioKeyHandler(PlayScene* s) : KeyEventHandler() {
		scene = s;
	}

	virtual void KeyState(BYTE* states) {
		LPGAMEOBJECT mario = scene->GetPlayer();
		if (mario == NULL) return;

		if (mario->GetState() == MARIO_STATE::DIE) return;

		int moveDir = 0;
		if (states[VK_RIGHT] & 0x80)
			moveDir = 1;
		else if (states[VK_LEFT] & 0x80)
			moveDir = -1;

		bool jumpPressed = (states[VK_SPACE] & 0x80) != 0;
		bool jumpJustPressed = jumpPending;

		bool runHeld = (moveDir != 0);

		bool sitPressed = (states[VK_DOWN] & 0x80) != 0;

		Mario* marioObj = dynamic_cast<Mario*>(mario);
		if (marioObj != nullptr)
		{
			marioObj->SetPhysicsInput(moveDir, jumpPressed, jumpJustPressed, runHeld);
		}
		jumpPending = false;

		if (sitPressed)
		{
			mario->SetState(MARIO_STATE::SIT);
		}
		else if (moveDir != 0)
		{
			if (moveDir > 0)
				mario->SetState(MARIO_STATE::WALKING_RIGHT);
			else
				mario->SetState(MARIO_STATE::WALKING_LEFT);
		}
		else
		{
			mario->SetState(MARIO_STATE::IDLE);
		}
	}

	virtual void OnKeyDown(int KeyCode) {
		if (KeyCode == 'P') {
			GameManager::GetInstance()->SetGameState(GAME_STATE::PAUSE);
			return;
		}

		LPGAMEOBJECT mario = scene->GetPlayer();
		if (mario == NULL) return;

		if (mario->GetState() == MARIO_STATE::DIE) return;

		switch (KeyCode)
		{
		case 'G':
		{
			flyMode = !flyMode;
			Mario* marioObj = dynamic_cast<Mario*>(mario);
			if (marioObj != nullptr)
			{
				marioObj->SetFlyMode(flyMode);
				if (flyMode)
				{
					marioObj->SetInvincible(999999999);
					DebugOut(L"[DEBUG] Fly mode ON\n");
				}
				else
				{
					marioObj->SetInvincible(0);
					DebugOut(L"[DEBUG] Fly mode OFF\n");
				}
			}
			break;
		}
		case VK_SHIFT:
			DebugOut(L"[KEYBOARD] Key 'Shift' pressed: Shooting bullet\n");
			mario->SetState(MARIO_STATE::SHOOT);
			break;
		case VK_SPACE:
			jumpPending = true;
			mario->SetState(MARIO_STATE::JUMP);
			break;
		}
	}

	virtual void OnKeyUp(int KeyCode) {
	}
};
