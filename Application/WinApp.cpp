#include "WinApp.h"
#include "Renderer.h"
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

    // 2. Tạo cửa sổ
    m_hwnd = CreateWindowEx(0, L"WindowClass", L"C++ DirectX Game Engine",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        width, height, NULL, NULL, m_hInstance, this); // Truyền 'this' để static proc có thể truy cập

    if (!m_hwnd) return false;

    Renderer::GetInstance()->Init(m_hwnd, hInstance);
    ShowWindow(m_hwnd, SW_SHOW);
    m_isRunning = true;
    return true;
}

int WinApp::Run() {
    MSG msg = { 0 };
    ULONGLONG frameStart = GetTickCount64();
    ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;
    // ĐÂY LÀ GAME LOOP
    while (m_isRunning) {
        // Kiểm tra tin nhắn từ Windows (như đóng cửa sổ, nhấn phím)
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                m_isRunning = false;
        }
        ULONGLONG now = GetTickCount64();

        // dt: the time between (beginning of last frame) and now
        // this frame: the frame we are about to render
        ULONGLONG dt = now - frameStart;

        if (dt >= tickPerFrame)
        {
            frameStart = now;
            Update((DWORD)dt);
            Render();
        }
        else
            Sleep((DWORD)(tickPerFrame - dt));
    }
    return (int)msg.wParam;
}

void WinApp::Update(float deltaTime) {
    // Gọi m_pWorld->Update(deltaTime)
}

void WinApp::Render() {
    // Gọi m_pRenderer->Draw(...)
    Renderer::GetInstance()->BeginRender();
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