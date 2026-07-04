#pragma once
#include "GameObject.h"

namespace ENEMY_TURN_BLOCK {
	constexpr float WIDTH = 16.0f;
	constexpr float HEIGHT = 16.0f;
	constexpr ULONGLONG TURN_COOLDOWN = 2000; // ms: min time between two flips of the same enemy
}

// Block vô hình chỉ ảnh hưởng đến enemy
// Enemy đè lên block sẽ đảo hướng, nếu vẫn đè thì đảo lại mỗi ~2s (cooldown)
// Mario và các đối tượng khác lọt qua bình thường
class EnemyTurnBlock : public GameObject {
public:
	EnemyTurnBlock(float x, float y, float z) : GameObject(x, y, z) {};
	virtual ~EnemyTurnBlock() {};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override {}                       // Vô hình
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	bool IsCollidable() override { return false; }  // Xử lý thủ công, không chặn ai cả
	bool IsBlocking() override { return false; }
};
