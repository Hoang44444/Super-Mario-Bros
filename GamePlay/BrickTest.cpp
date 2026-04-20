#include "BrickTest.h"
#include "debug.h"
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
	if (texture != NULL)
		Renderer::GetInstance()->Draw(x, y, texture);
	else {
		// Fallback: Nếu không có texture, ta vẫn có thể debug bằng cách log vị trí (hạn chế log để tránh lag)
		static DWORD lastLog = 0;
		if (GetTickCount() - lastLog > 2000) {
			DebugOut(L"[WARNING] Brick::Render: Texture is NULL at (%.1f, %.1f)\n", x, y);
			lastLog = GetTickCount();
		}
	}
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