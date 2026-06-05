#pragma once
#include "Enemy.h"

#define BULLET_BILL_WIDTH 16.0f
#define BULLET_BILL_HEIGHT 12.0f
#define BULLET_BILL_SPEED 0.2f

class BulletBill : public Enemy
{
public:
	BulletBill(float x, float y, float z);
	virtual ~BulletBill() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	virtual void OnMarioCollison(Mario* mario) override;

	// Set direction and velocity for this bullet
	void SetMovement(int dir);
};