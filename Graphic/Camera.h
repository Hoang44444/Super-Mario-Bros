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

    // --- Earthquake / camera-shake state ---
    bool  quakeEnabled = false;   // earthquake đang được lên lịch cho màn này
    bool  quakeShaking = false;   // đang trong đợt rung (2 giây) hay đang nghỉ
    float quakePhaseTimer = 0;    // ms đã trôi qua trong pha hiện tại (rung hoặc nghỉ)
    float quakeElapsed = 0;       // ms trôi qua trong đợt rung hiện tại (dùng cho hàm sin)
    float shakeOffsetX = 0;       // độ lệch camera do rung (px), cộng vào GetX/GetY
    float shakeOffsetY = 0;

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

    // --- Earthquake / camera-shake ---
    void StartEarthquake();           // bắt đầu chu kỳ rung (rung ngay lập tức)
    void StopEarthquake();            // dừng hẳn, đưa camera về vị trí gốc
    void UpdateEarthquake(float dt);  // cập nhật chu kỳ rung mỗi frame
    bool IsEarthquakeActive() const { return quakeEnabled; }
};

