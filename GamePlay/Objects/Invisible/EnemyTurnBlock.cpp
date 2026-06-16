#include "EnemyTurnBlock.h"
#include "Enemy.h"

void EnemyTurnBlock::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + ENEMY_TURN_BLOCK::WIDTH;
	b = y + ENEMY_TURN_BLOCK::HEIGHT;
}

void EnemyTurnBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (coObjects == NULL) return;

	float bl, bt, br, bb;
	GetBoundingBox(bl, bt, br, bb);

	for (LPGAMEOBJECT obj : *coObjects) {
		if (obj == NULL || obj->IsDeleted()) continue;

		// Only enemies are affected; everything else (Mario, items, ...) is ignored.
		Enemy* enemy = dynamic_cast<Enemy*>(obj);
		if (enemy == NULL) continue;

		float el, et, er, eb;
		enemy->GetBoundingBox(el, et, er, eb);

		// AABB overlap test
		bool overlap = !(er < bl || el > br || eb < bt || et > bb);
		if (!overlap) continue;

		// Flip on contact, throttled so it turns at most once per cooldown window.
		enemy->ReverseDirection(ENEMY_TURN_BLOCK::TURN_COOLDOWN);
	}
}
