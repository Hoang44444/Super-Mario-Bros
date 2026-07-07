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

		// Chỉ enemy bị ảnh hưởng; các đối tượng khác (Mario, items, ...) bị bỏ qua
		Enemy* enemy = dynamic_cast<Enemy*>(obj);
		if (enemy == NULL) continue;

		// Bỏ qua enemy đang sống nhưng không di chuyển (shell idle, ẩn, ...) hoặc đang chết
		if (!enemy->CanBeTurnedByBlock()) continue;

		float el, et, er, eb;
		enemy->GetBoundingBox(el, et, er, eb);

		// Kiểm tra AABB overlap
		bool overlap = !(er < bl || el > br || eb < bt || et > bb);
		if (!overlap) continue;

		// Đảo hướng khi chạm, giới hạn để đảo tối đa 1 lần mỗi cooldown
		enemy->ReverseDirection(ENEMY_TURN_BLOCK::TURN_COOLDOWN);
	}
}
