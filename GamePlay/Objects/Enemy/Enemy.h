#pragma once
#include "GameObject.h"
#include "Mario.h"

#define ENEMY_AUTO_TURN_INTERVAL 3000 // ms an enemy walks in one direction before turning around

class Enemy : public GameObject{
protected:
	ULONGLONG lastTurnTime = 0; // last time direction was flipped by a turn block (ms)
	ULONGLONG directionStartTime = 0; // when the current direction started (ms), for AutoTurnAround
	int lastAutoDirection = 0;        // direction tracked by AutoTurnAround to detect external flips
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
	virtual void OnHitByBullet() { Delete(); }

	// Flip direction automatically after walking ENEMY_AUTO_TURN_INTERVAL (~3s) in one
	// direction. If the direction was changed by something else (a turn block, hitting a
	// wall), the timer restarts so the count is always relative to the current direction.
	// Call this every frame from the enemy's Update().
	void AutoTurnAround() {
		ULONGLONG now = GetTickCount64();

		if (direction != lastAutoDirection) { // direction changed elsewhere -> restart the count
			lastAutoDirection = direction;
			directionStartTime = now;
			return;
		}

		if (now - directionStartTime >= ENEMY_AUTO_TURN_INTERVAL) {
			ReverseDirection();
			lastAutoDirection = direction; // keep in sync with the new direction
			directionStartTime = now;      // reset the count after turning
		}
	}
};

