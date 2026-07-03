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
	
	// A1/A4: một nguồn edge duy nhất — set từ OnKeyDown, consume trong KeyState
	bool jumpPending = false;

public:
	MarioKeyHandler(PlayScene* s) : KeyEventHandler() {
		scene = s;
	}

	virtual void KeyState(BYTE* states) {
		LPGAMEOBJECT mario = scene->GetPlayer();
		if (mario == NULL) return;

		if (mario->GetState() == MARIO_STATE::DIE) return;

		// Determine movement direction
		int moveDir = 0;
		if (states[VK_RIGHT] & 0x80)
			moveDir = 1;
		else if (states[VK_LEFT] & 0x80)
			moveDir = -1;

		// A4: jumpPressed = polling (variable jump height); jumpJustPressed = event jumpPending
		bool jumpPressed = (states[VK_SPACE] & 0x80) != 0;
		bool jumpJustPressed = jumpPending;

		// NES gốc: chạy = giữ phím hướng liên tục (không có phím Run; Shift = bắn)
		bool runHeld = (moveDir != 0);

		// Check sit button state (Down)
		bool sitPressed = (states[VK_DOWN] & 0x80) != 0;

		// Provide input to physics system
		Mario* marioObj = dynamic_cast<Mario*>(mario);
		if (marioObj != nullptr)
		{
			marioObj->SetPhysicsInput(moveDir, jumpPressed, jumpJustPressed, runHeld);
		}
		jumpPending = false; // A1: consume event edge sau khi đẩy sang Mario

		// Set animation state based on movement (for rendering)
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
		// 'P' pauses the game. While paused, input is handled by PauseKeyHandler
		// (GameManager routes by game state), so we only need to enter PAUSE here.
		if (KeyCode == 'P') {
			GameManager::GetInstance()->SetGameState(GAME_STATE::PAUSE);
			return;
		}

		LPGAMEOBJECT mario = scene->GetPlayer();
		if (mario == NULL) return;

		if (mario->GetState() == MARIO_STATE::DIE) return;

		// Handle non-physics actions (shooting)
		switch (KeyCode)
		{
		case VK_SHIFT:
			DebugOut(L"[KEYBOARD] Key 'Shift' pressed: Shooting bullet\n");
			mario->SetState(MARIO_STATE::SHOOT);
			break;
		case VK_SPACE:
			jumpPending = true; // A4: bắt tap nhanh (< 1 frame) không phụ thuộc polling
			mario->SetState(MARIO_STATE::JUMP);
			break;
		}
	}

	virtual void OnKeyUp(int KeyCode) {
		// A1: không reset edge nhảy ở đây — tránh xung đột với jumpPending
	}
};
