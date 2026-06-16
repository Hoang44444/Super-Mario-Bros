#pragma once
#include "GameObject.h"

namespace ENEMY_TURN_BLOCK {
	constexpr float WIDTH = 16.0f;
	constexpr float HEIGHT = 16.0f;
	constexpr ULONGLONG TURN_COOLDOWN = 2000; // ms: min time between two flips of the same enemy
}

// Invisible block that ONLY affects enemies. Any enemy overlapping the block
// reverses its direction; if it keeps overlapping it flips again every ~2s
// (the cooldown). Mario and every other object pass through untouched.
class EnemyTurnBlock : public GameObject {
public:
	EnemyTurnBlock(float x, float y, float z) : GameObject(x, y, z) {};
	virtual ~EnemyTurnBlock() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override {}                       // invisible
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	bool IsCollidable() override { return false; }  // handled manually, never blocks anyone
	bool IsBlocking() override { return false; }
};
