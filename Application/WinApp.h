#pragma once
#include <windows.h>
#include <string>

class WinApp {
public:
    WinApp();
    virtual ~WinApp();

    bool Initialize(HINSTANCE hInstance, int width, int height);

    int Run();

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hwnd;
    HINSTANCE m_hInstance;
    bool m_isRunning;

    void Update(float deltaTime);

    void Render();
};