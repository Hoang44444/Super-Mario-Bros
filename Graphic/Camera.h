#pragma once

#include "EarthquakeEffect.h"

// Camera quản lý viewport của game - xác định vùng nào của map được hiển thị
// Camera follow player và clamp không cho ra ngoài map
// Cách dùng:
//   1. Gọi Camera::GetInstance() để lấy singleton
//   2. SetSize() để set kích thước màn hình
//   3. SetMapSize() để set kích thước map
//   4. Follow() mỗi frame để camera follow player
//   5. GetX()/GetY() để lấy vị trí camera cho rendering
class Camera
{
private:
    static Camera* __instance;  // Instance singleton

    float x, y;                 // Vị trí camera (top-left của viewport)
    int screenWidth;            // Chiều rộng màn hình (pixel)
    int screenHeight;           // Chiều cao màn hình (pixel)

    int mapWidth;               // Chiều rộng map (pixel)
    int mapHeight;              // Chiều cao map (pixel)

    // Hiệu ứng động đất (rung camera)
    // Toàn bộ logic nằm trong EarthquakeEffect
    // Camera chỉ cộng offset của nó vào GetX/GetY
    EarthquakeEffect earthquake;

public:
    Camera();

    // Lấy instance singleton - tạo mới nếu chưa tồn tại
    static Camera* GetInstance();

    // Set vị trí camera trực tiếp (thường không dùng, dùng Follow() thay thế)
    void SetPosition(float x, float y);
    
    // Set kích thước màn hình (viewport)
    void SetSize(int width, int height);
    
    // Get kích thước màn hình
    void GetSize(int& width, int& height) { width = screenWidth; height = screenHeight; }
    
    // Set kích thước map (để clamp camera không ra ngoài)
    void SetMapSize(int width, int height);

    // Camera target (thường là player)
    // Đưa target vào giữa màn hình và clamp không ra ngoài map
    void Follow(float targetX, float targerY);

    // Lấy vị trí camera (có cộng offset earthquake nếu đang active)
    float GetX();
    float GetY();

    // --- Earthquake / camera-shake (uỷ quyền cho EarthquakeEffect) ---
    void StartEarthquake()           { earthquake.Start(); }
    void StopEarthquake()            { earthquake.Stop(); }
    void UpdateEarthquake(float dt)  { earthquake.Update(dt); }
    bool IsEarthquakeActive() const  { return earthquake.IsActive(); }
    bool IsEarthquakeShaking() const { return earthquake.IsShaking(); }
};

