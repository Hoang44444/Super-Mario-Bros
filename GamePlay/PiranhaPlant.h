#pragma once
#include "Enemy.h"

#define PIRANHA_PLANT_WIDTH 16.0f
#define PIRANHA_PLANT_HEIGHT 24.0f

#define PIRANHA_STATE_HIDING 0
#define PIRANHA_STATE_RISING 1
#define PIRANHA_STATE_SHOWING 2
#define PIRANHA_STATE_FALLING 3

#define PIRANHA_DELAY_TIME 2000 // Time in ms to stay hiding or showing
#define PIRANHA_MOVE_SPEED 0.03f // Speed of rising/falling

class PiranhaPlant : public Enemy
{
private:
	float originalY;
	float hideY;
	float showY;
	DWORD timer;

public:
	PiranhaPlant(float x, float y, float z);
	virtual ~PiranhaPlant() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	virtual void OnMarioCollison(Mario* mario, float ny) override;
};