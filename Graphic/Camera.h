#pragma once
#pragma once

class Camera
{
private:
    static Camera* __instance;

    float x, y;               // vị trí camera (world)
    int screenWidth;
    int screenHeight;

    int mapWidth;
    int mapHeight;

public:
    Camera();

    static Camera* GetInstance();

    void SetPosition(float x, float y);
    void SetSize(int width, int height);
    void SetMapSize(int width, int height);

    void Follow(float targetX, float targetY);

    float GetX();
    float GetY();
};

