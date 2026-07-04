#include "Camera.h"
#include <algorithm>

// Khởi tạo instance singleton tĩnh
Camera* Camera::__instance = NULL;

Camera::Camera()
{
    x = y = 0;
    screenWidth = screenHeight = 0;
    mapWidth = mapHeight = 0;
}

Camera* Camera::GetInstance()
{
    // Lấy instance singleton - tạo mới nếu chưa tồn tại
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
	// Trả về vị trí X có cộng offset earthquake (nếu đang rung)
	return x + earthquake.GetOffsetX();
}

float Camera::GetY()
{
	// Trả về vị trí Y có cộng offset earthquake (nếu đang rung)
	return y + earthquake.GetOffsetY();
}

void Camera::Follow(float targetX, float targetY)
{
    // Bước 1: Đưa target về giữa màn hình
    x = targetX - screenWidth / 2;
    y = targetY - screenHeight / 2;
    
    // Bước 2: Clamp không cho camera ra ngoài map
    // Nếu map nhỏ hơn màn hình, max sẽ là 0 (không clamp âm)
    float max_x = std::max(0.0f, (float)(mapWidth - screenWidth));
	float max_y = std::max(0.0f, (float)(mapHeight - screenHeight));

    // Clamp x và y trong khoảng [0, max]
    x = std::max(0.0f, std::min(x, max_x));
	y = std::max(0.0f, std::min(y, max_y));
}
