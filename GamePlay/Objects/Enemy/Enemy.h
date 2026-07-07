#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "ScoreManager.h"

class Enemy : public GameObject{
protected:
	ULONGLONG lastTurnTime = 0;
public:
	Enemy(float x, float y, float z) : GameObject(x, y, z) {};
	virtual ~Enemy() {};

	bool IsCollidable() { return  true; }
	bool IsBlocking() { return false; }

	void ReverseDirection() {
		direction = -direction;
		vx = -vx;
	}

	bool ReverseDirection(ULONGLONG cooldownMs) {
		ULONGLONG now = GetTickCount64();
		if (now - lastTurnTime < cooldownMs) return false;
		lastTurnTime = now;
		ReverseDirection();
		return true;
	}

	virtual void OnMarioCollison(Mario* mario, float ny) = 0;

	virtual void OnHitByBullet() {
		ScoreManager::Get().AddScore(SCORE_VALUES::FIREBALL_KILL);
		Delete();
	}

	virtual void OnHitByStar(Mario* mario) {
		ScoreManager& scoreMgr = ScoreManager::Get();
		scoreMgr.IncrementCombo();

		if (scoreMgr.IsMaxCombo())
		{
			mario->AddLife(1);
		}
		else
		{
			mario->AddScore(scoreMgr.GetComboScore());
		}

		Delete();
	}

	virtual bool CanBeTurnedByBlock() { return IsCollidable() && vx != 0; }
};

