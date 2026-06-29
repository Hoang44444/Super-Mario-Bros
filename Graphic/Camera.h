#pragma once

#include "EarthquakeEffect.h"

class Camera
{
private:
    static Camera* __instance;

    float x, y;
    int screenWidth;
    int screenHeight;

    int mapWidth;
    int mapHeight;

    // Hiệu ứng động đất (rung camera). Toàn bộ logic nằm trong EarthquakeEffect;
    // Camera chỉ cộng offset của nó vào GetX/GetY.
    EarthquakeEffect earthquake;

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

    // --- Earthquake / camera-shake (uỷ quyền cho EarthquakeEffect) ---
    void StartEarthquake()           { earthquake.Start(); }
    void StopEarthquake()            { earthquake.Stop(); }
    void UpdateEarthquake(float dt)  { earthquake.Update(dt); }
    bool IsEarthquakeActive() const  { return earthquake.IsActive(); }
};

