#pragma once
#include <windows.h>
#include <string>

// Class WinApp quản lý cửa sổ Windows và vòng lặp game chính
// Đây là entry point của ứng dụng game
// Cách dùng:
//   1. Tạo instance WinApp trong WinMain
//   2. Gọi Initialize() để tạo cửa sổ và khởi tạo các hệ thống
//   3. Gọi Run() để bắt đầu vòng lặp game
class WinApp {
public:
    WinApp();
    virtual ~WinApp();

    // Khởi tạo cửa sổ Windows và các hệ thống game (Renderer, GameManager, Camera)
    // width/height: kích thước cửa sổ mặc định (sẽ bị override bởi settings trong file config)
    bool Initialize(HINSTANCE hInstance, int width, int height);

    // Chạy vòng lặp game chính (game loop)
    // Vòng lặp này xử lý input, update logic, và render mỗi frame
    // Trả về khi cửa sổ đóng (WM_QUIT)
    int Run();

    // Window procedure callback - xử lý tin nhắn từ Windows
    // Được gọi bởi Windows khi có sự kiện (keydown, keyup, destroy, v.v.)
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hwnd;           // Handle cửa sổ Windows
    HINSTANCE m_hInstance; // Instance của ứng dụng
    bool m_isRunning;      // Flag để kiểm tra game có đang chạy không

    // Update logic game mỗi frame
    // deltaTime: thời gian giữa 2 frame (ms) - dùng cho animation và physics
    void Update(float deltaTime);
    
    // Render frame hiện tại
    void Render();
};