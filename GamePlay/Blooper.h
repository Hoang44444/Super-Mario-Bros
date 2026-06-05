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

class Blooper : public Enemy
{
private:
	DWORD timer;
	float startY;
	bool flipX;
	float chaseDistance;

public:
	Blooper(float x, float y, float z);
	virtual ~Blooper() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	virtual void OnMarioCollison(Mario* mario) override;
};