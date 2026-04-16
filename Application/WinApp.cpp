#include "WinApp.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "PlayScene.h"

#define MAX_FRAME_RATE 60

WinApp::WinApp() : m_hwnd(nullptr), m_hInstance(nullptr), m_isRunning(false) {
}

WinApp::~WinApp() {
}

bool WinApp::Initialize(HINSTANCE hInstance, int width, int height) {
    m_hInstance = hInstance;

    // 1. Đăng ký lớp cửa sổ
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = m_hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";
    RegisterClassEx(&wc);

    RECT rect = { 0, 0, 1280, 720 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    // 2. Tạo cửa sổ
    m_hwnd = CreateWindowEx(0, L"WindowClass", L"C++ DirectX Game Engine",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        windowWidth, windowHeight, NULL, NULL, m_hInstance, this);

    if (!m_hwnd) return false;

    Renderer::GetInstance()->Init(m_hwnd, hInstance);
    ShowWindow(m_hwnd, SW_SHOW);                  
    UpdateWindow(m_hwnd);
    // Initialise SceneManager and PlayScene
    SceneManager::GetInstance()->Add(1, new PlayScene(1, L""));
    SceneManager::GetInstance()->SwitchScene(1);

    m_isRunning = true;
    return true;
}

int WinApp::Run() {
    MSG msg = { 0 };
    ULONGLONG frameStart = GetTickCount64();
    ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;
    // ĐÂY LÀ GAME LOOP
    while (m_isRunning) {
        // Kiểm tra tin nhắn từ Windows
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                m_isRunning = false;
        }
        ULONGLONG now = GetTickCount64();

        ULONGLONG dt = now - frameStart;

        if (dt >= tickPerFrame)
        {
            frameStart = now;
            Update((float)dt);
            Render();
        }
        else
            Sleep((DWORD)(tickPerFrame - dt));
    }
    return (int)msg.wParam;
}

void WinApp::Update(float deltaTime) {
    SceneManager::GetInstance()->Update((DWORD)deltaTime);
}

void WinApp::Render() {
    Renderer::GetInstance()->BeginRender();

    SceneManager::GetInstance()->Render();

    Renderer::GetInstance()->EndRender();
}

// Xử lý sự kiện cửa sổ
LRESULT CALLBACK WinApp::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}