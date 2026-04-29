#pragma once
#include "KeyEventHandler.h"
#include "PlayScene.h"
#include "Mario.h"
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

		if (mario->GetState() == MARIO_STATE_DIE) return;

		if (states[VK_RIGHT] & 0x80)
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		else if (states[VK_LEFT] & 0x80)
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		else
			mario->SetState(MARIO_STATE_IDLE);
	}

	virtual void OnKeyDown(int KeyCode) {
		LPGAMEOBJECT mario = scene->GetPlayer();
		if (mario == NULL) return;

		if (mario->GetState() == MARIO_STATE_DIE) return;

		switch (KeyCode)
		{
		case VK_SPACE:
			mario->SetState(MARIO_STATE_JUMP);
			break;
		case 'k':
			mario->SetState(MARIO_STATE_SHOOT);
			break;
		}
	}

	virtual void OnKeyUp(int KeyCode) {
		// Handle key release if needed
	}
};
