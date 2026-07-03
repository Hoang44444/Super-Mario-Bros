#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "../Core/ScoreManager.h"

class Enemy : public GameObject{
protected:
	ULONGLONG lastTurnTime = 0; // last time direction was flipped by a turn block (ms)
public:
	Enemy(float x, float y, float z) : GameObject(x, y, z) {};
	virtual ~Enemy() {};

	bool IsCollidable() { return  true; }
	bool IsBlocking() { return false; }

	// Flip both facing and horizontal velocity, so enemies that only set their
	// speed inside SetState() still turn around immediately.
	void ReverseDirection() {
		direction = -direction;
		vx = -vx;
	}

	// Reverse only if at least cooldownMs has elapsed since the last flip.
	// Used by EnemyTurnBlock so an enemy overlapping the block flips at most
	// once per cooldown window (and keeps flipping every cooldown if it stays).
	// Returns true if it actually reversed this call.
	bool ReverseDirection(ULONGLONG cooldownMs) {
		ULONGLONG now = GetTickCount64();
		if (now - lastTurnTime < cooldownMs) return false;
		lastTurnTime = now;
		ReverseDirection();
		return true;
	}

	virtual void OnMarioCollison(Mario* mario, float ny) = 0; // Define this in derived classes to specify what happens when Mario collides with the enemy

	// Called when a bullet hits this enemy. Default: just remove the enemy.
	// Enemies that have a death animation override this to switch to their die state instead.
	virtual void OnHitByBullet() { 
		// Award score for fireball kill (no combo for fireball/star kills)
		ScoreManager::Get().AddScore(SCORE_VALUES::FIREBALL_KILL);
		Delete(); 
	}

	// Called when star power kills this enemy
	virtual void OnHitByStar(Mario* mario) {
		// Award score with combo system for star kills
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

	// Whether a turn block (id 44) is allowed to flip this enemy. Only enemies that are
	// alive (IsCollidable() is false in death states) and actually moving (vx != 0) get
	// turned; enemies that are alive but standing still (idle shell, hidden, ...) are left alone.
	virtual bool CanBeTurnedByBlock() { return IsCollidable() && vx != 0; }
};

