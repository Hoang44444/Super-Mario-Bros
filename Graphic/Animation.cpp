#include "Animation.h"
#include "Renderer.h"

void Animation::Add(LPSPRITE sprite, DWORD time)
{
	// Thêm frame mới vào danh sách
	frames.push_back(new AnimationFrame(sprite, time));
}

void Animation::Render(float x, float y, float z)
{
	// Delegate cho phiên bản với flipX = false
	Render(x, y, z, false);
}

void Animation::Render(float x, float y, float z, bool flipX)
{
	if (frames.size() == 0) return;

	DWORD now = GetTickCount64();

	// Nếu là lần render đầu tiên, bắt đầu từ frame 0
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	// Nếu có nhiều hơn 1 frame, kiểm tra xem có cần chuyển frame không
	else if (frames.size() > 1) 
	{
		DWORD t = frames[currentFrame]->GetTime();
		// Nếu đã đủ thời gian hiển thị frame hiện tại, chuyển sang frame tiếp theo
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			// Loop về frame đầu nếu đã hết
			if (currentFrame >= frames.size())
				currentFrame = 0;

			lastFrameTime = now;
		}
	}

	// Lấy sprite của frame hiện tại
	LPSPRITE sprite = frames[currentFrame]->GetSprite();
	if (sprite == nullptr) return;

	RECT rect = sprite->GetRect();
	if (sprite->GetTexture() == nullptr) return;

	// Render sprite qua Renderer
	Renderer::GetInstance()->Draw(
		x, y, z,
		sprite->GetTexture(),
		&rect,
		1.0f,  // alpha
		flipX
	);
}

void Animation::RenderScaled(float x, float y, float z, float destWidth, float destHeight)
{
	if (frames.size() == 0) return;

	// Nếu là lần render đầu tiên, bắt đầu từ frame 0
	if (currentFrame == -1)
		currentFrame = 0;

	LPSPRITE sprite = frames[currentFrame]->GetSprite();
	if (sprite == nullptr || sprite->GetTexture() == nullptr) return;

	RECT rect = sprite->GetRect();

	// Render với kích thước tùy chỉnh (scale)
	Renderer::GetInstance()->DrawScaled(
		x, y, z,
		sprite->GetTexture(),
		destWidth, destHeight,
		&rect,
		1.0f,  // alpha
		false  // không flipX cho background
	);
}