#include "Camera.h"
#include <algorithm>
#include <cmath>

namespace
{
    constexpr float QUAKE_SHAKE_MS    = 5000.0f;    // mỗi đợt rung kéo dài 5 giây
    constexpr float QUAKE_INTERVAL_MS = 10000.0f;   // nghỉ 10 giây trước đợt rung kế tiếp
    constexpr float QUAKE_AMPLITUDE_Y = 4.0f;       // biên độ rung dọc (px)
    constexpr float QUAKE_AMPLITUDE_X = 2.0f;       // biên độ rung ngang (px)
    constexpr float QUAKE_OMEGA_Y     = 0.060f;     // tần số góc dọc (rad/ms) ~ 9.5 Hz
    constexpr float QUAKE_OMEGA_X     = 0.045f;     // tần số góc ngang (rad/ms) ~ 7.2 Hz
}

Camera* Camera::__instance = NULL;

Camera::Camera()
{
    x = y = 0;
    screenWidth = screenHeight = 0;
    mapWidth = mapHeight = 0;
}

Camera* Camera::GetInstance()
{
    if (__instance == NULL)
        __instance = new Camera();
    return __instance;
}

void Camera::SetPosition(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Camera::SetSize(int width, int height)
{
    this->screenWidth = width;
    this->screenHeight = height;
}

void Camera::SetMapSize(int width, int height)
{
    this->mapWidth = width;
    this->mapHeight = height;
}

float Camera::GetX()
{
	return x + shakeOffsetX;
}

float Camera::GetY()
{
	return y + shakeOffsetY;
}

void Camera::StartEarthquake()
{
	if (quakeEnabled) return;   // đã chạy rồi thì không reset chu kỳ
	quakeEnabled    = true;
	quakeShaking    = true;     // rung ngay từ đầu
	quakePhaseTimer = 0;
	quakeElapsed    = 0;
}

void Camera::StopEarthquake()
{
	quakeEnabled    = false;
	quakeShaking    = false;
	quakePhaseTimer = 0;
	quakeElapsed    = 0;
	shakeOffsetX    = 0;
	shakeOffsetY    = 0;
}

void Camera::UpdateEarthquake(float dt)
{
	if (!quakeEnabled)
	{
		shakeOffsetX = shakeOffsetY = 0;
		return;
	}

	quakePhaseTimer += dt;

	if (quakeShaking)
	{
		quakeElapsed += dt;

		if (quakePhaseTimer >= QUAKE_SHAKE_MS)
		{
			// hết 2 giây rung -> chuyển sang nghỉ
			quakeShaking    = false;
			quakePhaseTimer = 0;
			shakeOffsetX = shakeOffsetY = 0;
		}
		else
		{
			// envelope giảm dần để đợt rung kết thúc êm (không giật về 0 đột ngột)
			float k = 1.0f - quakePhaseTimer / QUAKE_SHAKE_MS;
			shakeOffsetY = QUAKE_AMPLITUDE_Y * k * sinf(QUAKE_OMEGA_Y * quakeElapsed);
			shakeOffsetX = QUAKE_AMPLITUDE_X * k * sinf(QUAKE_OMEGA_X * quakeElapsed);
		}
	}
	else
	{
		shakeOffsetX = shakeOffsetY = 0;

		if (quakePhaseTimer >= QUAKE_INTERVAL_MS)
		{
			// hết 10 giây nghỉ -> rung tiếp
			quakeShaking    = true;
			quakePhaseTimer = 0;
			quakeElapsed    = 0;
		}
	}
}

void Camera::Follow(float targetX, float targetY)
{
    // Đưa player về giữa màn hình
    x = targetX - screenWidth / 2;
    y = targetY - screenHeight / 2;
    
    // Clamp không cho ra ngoài map
    // Ensure we don't clamp to a negative value if map is smaller than screen
    float max_x = std::max(0.0f, (float)(mapWidth - screenWidth));
	float max_y = std::max(0.0f, (float)(mapHeight - screenHeight));

    x = std::max(0.0f, std::min(x, max_x));
	y = std::max(0.0f, std::min(y, max_y));
}
