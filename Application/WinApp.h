#pragma once
#include <windows.h>
#include <string>

class WinApp {
public:
    WinApp();
    virtual ~WinApp();

    // Khởi tạo cửa sổ và các hệ thống
    bool Initialize(HINSTANCE hInstance, int width, int height);

    // Chạy vòng lặp game
    int Run();

    // Các hàm xử lý tin nhắn từ Windows
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hwnd;
    HINSTANCE m_hInstance;
    bool m_isRunning;

    // Các hàm vòng đời chính
    void Update(float deltaTime);
    void Render();
};