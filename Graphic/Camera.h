#pragma once
#pragma once

class Camera
{
private:
    static Camera* __instance;

    float x, y;              
    int screenWidth;
    int screenHeight;

    int mapWidth;
    int mapHeight;

public:
    Camera();

    static Camera* GetInstance();

    void SetPosition(float x, float y);
    void SetSize(int width, int height);
    void GetSize(int& width, int& height) { width = screenWidth; height = screenHeight; }
    void SetMapSize(int width, int height);

    void Follow(float targetX, float targerY);

    float GetX();
    float GetY();
};

