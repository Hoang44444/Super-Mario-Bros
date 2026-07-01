#pragma once
#include "StaticObject.h"
#include "AssetID.h"

constexpr float QUESTION_BLOCK_BBOX_SIZE = 14.0f;

namespace QUESTION_BLOCK_STATE
{
	constexpr int ACTIVE  = 0;
	constexpr int DELETED = 1;
}

class QuestionBlock : public StaticObject {
private:
	int itemType;        // OBJECT type id of the item released when hit
	void Break();
	void SpawnItem(Mario* mario);
public:
	QuestionBlock(float x, float y, float z, int itemType = OBJECT::COIN) : StaticObject(x, y, z) {
		this->state = QUESTION_BLOCK_STATE::ACTIVE;
		this->itemType = itemType;
	}
	virtual ~QuestionBlock() {}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void OnMarioCollision(Mario* mario, LPCOLLISIONEVENT e) override;
};
