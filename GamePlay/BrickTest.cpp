#include "BrickTest.h"

void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//// Move the brick left and right between the limits
	//x += vx * dt;
	//if (x < leftLimit) {
	//	x = leftLimit;
	//	vx = -vx; // Reverse direction
	//}
	//else if (x > rightLimit) {
	//	x = rightLimit;
	//	vx = -vx; // Reverse direction
	//}

	this->x = 0;
	this->y = 0;

	wchar_t buffer[128];
	swprintf_s(buffer, 128, L"x = %d, y = %d\n", this->x, this->y);
	OutputDebugString(buffer);

}

void Brick::Render()
{
	OutputDebugString(L"[RENDER] Brick::Render called!\n");
	if (texture != NULL)
		Renderer::GetInstance()->Draw(x, y, texture);
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	if (texture != NULL) {
		r = x + texture->getWidth();
		b = y + texture->getHeight();
	}
	else {
		r = x + 16.0f; // Default width
		b = y + 16.0f; // Default height
	}
}