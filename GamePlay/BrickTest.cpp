#include "BrickTest.h"
#include "Animation.h"
#include "AnimationManager.h"

#define ID_SPRITE_BRICK 10001

void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Move the brick left and right between the limits
	x += vx * dt;
	if (x < leftLimit) {
		x = leftLimit;
		vx = -vx; // Change direction
	}
	else if (x > rightLimit) {
		x = rightLimit;
		vx = -vx; // Change direction
	}
}

void Brick::Render()
{
	LPANIMATION ani = AnimationManager::GetInstance()->Get();
	ani->Render(x, y);
}