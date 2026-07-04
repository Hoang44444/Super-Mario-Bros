#pragma once
#include "KeyEventHandler.h"
#include "PlayScene.h"
#include "Mario.h"
#include "GameManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"

// Xử lý input bàn phím cho Mario trong gameplay
// Chuyển đổi input người chơi thành hành động của Mario (di chuyển, nhảy, bắn, pause)
class MarioKeyHandler : public KeyEventHandler
{
	PlayScene* scene;

	// A1/A4: một nguồn edge duy nhất — set từ OnKeyDown, consume trong KeyState
	bool jumpPending = false;

	// Debug: Fly mode toggle (kháng sát thương, bay ở y=150, không flicker)
	bool flyMode = false;

public:
	MarioKeyHandler(PlayScene* s) : KeyEventHandler() {
		scene = s;
	}

	virtual void KeyState(BYTE* states) {
		LPGAMEOBJECT mario = scene->GetPlayer();
		if (mario == NULL) return;

		if (mario->GetState() == MARIO_STATE::DIE) return;

		// Xác định hướng di chuyển
		int moveDir = 0;
		if (states[VK_RIGHT] & 0x80)
			moveDir = 1;
		else if (states[VK_LEFT] & 0x80)
			moveDir = -1;

		// A4: jumpPressed = polling (chiều cao nhảy biến đổi); jumpJustPressed = event jumpPending
		bool jumpPressed = (states[VK_SPACE] & 0x80) != 0;
		bool jumpJustPressed = jumpPending;

		// NES gốc: chạy = giữ phím hướng liên tục (không có phím Run; Shift = bắn)
		bool runHeld = (moveDir != 0);

		// Kiểm tra trạng thái phím ngồi (Down)
		bool sitPressed = (states[VK_DOWN] & 0x80) != 0;

		// Cung cấp input cho hệ thống physics
		Mario* marioObj = dynamic_cast<Mario*>(mario);
		if (marioObj != nullptr)
		{
			marioObj->SetPhysicsInput(moveDir, jumpPressed, jumpJustPressed, runHeld);
		}
		jumpPending = false; // A1: consume event edge sau khi đẩy sang Mario

		// Set trạng thái animation dựa trên di chuyển (để render)
		// LƯU Ý: Khi Mario DEAD, hàm này return sớm (dòng 25),
		// nên không có thay đổi trạng thái animation trong lúc chết.
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
		// 'P' pause game. Khi pause, input được xử lý bởi PauseKeyHandler
		// (GameManager route theo game state), nên chỉ cần vào PAUSE ở đây.
		if (KeyCode == 'P') {
			GameManager::GetInstance()->SetGameState(GAME_STATE::PAUSE);
			return;
		}

		LPGAMEOBJECT mario = scene->GetPlayer();
		if (mario == NULL) return;

		if (mario->GetState() == MARIO_STATE::DIE) return;

		// Xử lý các hành động không phải physics (bắn)
		// LƯU Ý: Khi Mario DEAD, hàm này return sớm (dòng 81),
		// nên không có input ảnh hưởng trạng thái hoặc tốc độ Mario trong lúc chết.
		switch (KeyCode)
		{
		case 'G':
		{
			// Debug: Toggle fly mode (kháng sát thương, bay ở y=150, không flicker)
			flyMode = !flyMode;
			Mario* marioObj = dynamic_cast<Mario*>(mario);
			if (marioObj != nullptr)
			{
				marioObj->SetFlyMode(flyMode);
				if (flyMode)
				{
					marioObj->SetInvincible(999999999); // Kháng sát thương vô thời hạn
				}
				else
				{
					marioObj->SetInvincible(0); // Tắt miễn thương
				}
			}
			break;
		}
		case VK_SHIFT:
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
