#pragma once
#include "GameObject.h"

namespace ENEMY_TURN_BLOCK {
	constexpr float WIDTH = 16.0f;
	constexpr float HEIGHT = 16.0f;
	constexpr ULONGLONG TURN_COOLDOWN = 2000;
}

class EnemyTurnBlock : public GameObject {
public:
	EnemyTurnBlock(float x, float y, float z) : GameObject(x, y, z) {};
	virtual ~EnemyTurnBlock() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }
};
