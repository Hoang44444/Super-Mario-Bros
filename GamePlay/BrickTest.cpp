#include "BrickTest.h"
#include "debug.h"
#include "AnimationManager.h"
void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//// Move the brick left and right between the limits
	x += vx * dt;
	if (x < leftLimit) {
		x = leftLimit;
		vx = -vx; // Reverse direction
	}
	else if (x > rightLimit) {
		x = rightLimit;
		vx = -vx; // Reverse direction
	}

	wchar_t buffer[128];
	swprintf_s(buffer, 128, L"[BRICK POSITION]x = %f, y = %f\n", this->x, this->y);
	OutputDebugString(buffer);

}

void Brick::Render()
{
	AnimationManager* animations = AnimationManager::GetInstance();
	// For testing, we assume animation ID 100 is the brick
	LPANIMATION ani = animations->Get(100);
	if (ani != NULL)
		ani->Render(x, y);
	else
		DebugOut(L"[ERROR] Brick animation 100 not found\n");
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16.0f; // Default width
	b = y + 16.0f; // Default height
}