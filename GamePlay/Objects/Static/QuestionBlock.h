#pragma once
#include "StaticObject.h"

constexpr float QUESTION_BLOCK_BBOX_SIZE = 16.0f;

namespace QUESTION_BLOCK_STATE
{
	constexpr int ACTIVE  = 0;
	constexpr int DELETED = 1;
}

class QuestionBlock : public StaticObject {
private:
	void Break();
public:
	QuestionBlock(float x, float y, float z) : StaticObject(x, y, z) {
		this->state = 0;
	}
	virtual ~QuestionBlock() {}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void OnMarioCollision(Mario* mario, LPCOLLISIONEVENT e) override;
};
