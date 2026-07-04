#pragma once
#include "Enemy.h"

#define BLOOPER_WIDTH 16.0f
#define BLOOPER_HEIGHT 48.0f

#define BLOOPER_STATE_RISING 0
#define BLOOPER_STATE_SINKING 1
#define BLOOPER_STATE_IDLE 2

#define BLOOPER_RISE_TIME 800
#define BLOOPER_SINK_TIME 1500
#define BLOOPER_DETECT_DISTANCE 150.0f

// Blooper - enemy dưới nước (mực) di chuyển lên/xuống và đuổi theo Mario
class Blooper : public Enemy
{
private:
	DWORD timer;  // Timer cho state transitions
	float startY;  // Vị trí Y ban đầu (để reset khi idle)
	bool flipX;  // Flip animation theo hướng
	float chaseDistance;  // Khoảng cách để kích hoạt đuổi theo

public:
	Blooper(float x, float y, float z);
	virtual ~Blooper() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	virtual void OnMarioCollison(Mario* mario, float ny) override;
};