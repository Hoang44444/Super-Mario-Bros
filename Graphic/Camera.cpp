#include "Camera.h"
#include <algorithm>

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
	return x;
}

float Camera::GetY()
{
	return y;
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
