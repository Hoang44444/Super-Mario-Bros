#include "WinApp.h"
#include "Renderer.h"
#include "GameManager.h"
#include "Camera.h"
#include "debug.h"

// Giới hạn frame rate tối đa (FPS)
#define MAX_FRAME_RATE 60

WinApp::WinApp() : m_hwnd(nullptr), m_hInstance(nullptr), m_isRunning(false) {
	// Constructor - khởi tạo các thành viên mặc định
}

WinApp::~WinApp() {
	// Destructor - cleanup được thực hiện bởi các singleton classes
}

bool WinApp::Initialize(HINSTANCE hInstance, int width, int height) {
	m_hInstance = hInstance;

	// Bước 1: Load settings từ file config để lấy kích thước màn hình
	GameManager::GetInstance()->LoadSettings(L"super-mario-bros.txt");
	int screenWidth = GameManager::GetInstance()->GetScreenWidth();
	int screenHeight = GameManager::GetInstance()->GetScreenHeight();

	// Nếu file config không có kích thước, dùng giá trị mặc định
	if (screenWidth == 0) screenWidth = width;
	if (screenHeight == 0) screenHeight = height;

	// Bước 2: Đăng ký lớp cửa sổ Windows
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;  // Redraw khi resize
	wc.lpfnWndProc = WindowProc;           // Window procedure callback
	wc.hInstance = m_hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";
	RegisterClassEx(&wc);

	// Bước 3: Tính toán kích thước cửa sổ (bao gồm border, title bar)
	RECT rect = { 0, 0, screenWidth, screenHeight };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	// Bước 4: Tạo cửa sổ
	m_hwnd = CreateWindowEx(0, L"WindowClass", L"C++ DirectX Game Engine",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowWidth, windowHeight, NULL, NULL, m_hInstance, this);

	if (!m_hwnd) return false;

	// Bước 5: Hiển thị cửa sổ
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	// Bước 6: Khởi tạo các hệ thống game
	GameManager::GetInstance()->Init(m_hwnd, hInstance);
	Camera::GetInstance()->SetSize(Renderer::INTERNAL_SCREEN_WIDTH, Renderer::INTERNAL_SCREEN_HEIGHT);

	// Bước 7: Load game assets và scene từ file config
	GameManager::GetInstance()->Load(L"super-mario-bros.txt");

	m_isRunning = true;
	return true;
}

int WinApp::Run() {
	MSG msg = { 0 };
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;  // Thời gian mỗi frame (ms)
	
	// Vòng lặp game chính
	while (m_isRunning) {
		// Xử lý tin nhắn từ Windows (input, window events)
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				m_isRunning = false;
		}
		
		// Tính toán deltaTime (thời gian giữa 2 frame)
		ULONGLONG now = GetTickCount64();
		ULONGLONG dt = now - frameStart;

		// Chỉ update/render khi đủ thời gian cho 1 frame (giới hạn FPS)
		if (dt >= tickPerFrame)
		{
			frameStart = now;
			
			// Clamp dt để tránh physics bug khi load scene mới
			// Nếu load scene mất nhiều thời gian, dt sẽ rất lớn
			// mà không clamp, Mario sẽ bị "bắn" đi do physics dùng dt lớn
			if (dt > tickPerFrame) dt = tickPerFrame;
			
			GameManager::GetInstance()->ProcessKeyboard();  // Xử lý input keyboard
			Update((float)dt);  // Update game logic
			Render();            // Render frame
		}
		else
			// Nếu chưa đủ thời gian, sleep để tiết kiệm CPU
			Sleep((DWORD)(tickPerFrame - dt));
	}
	return (int)msg.wParam;
}

void WinApp::Update(float deltaTime) {
	// Delegate update logic cho GameManager
	GameManager::GetInstance()->Update((DWORD)deltaTime);
}

void WinApp::Render() {
	// Bắt đầu render (clear screen, setup render target)
	Renderer::GetInstance()->BeginRender();
	
	// Render game objects (delegated cho GameManager)
	GameManager::GetInstance()->Render();
	
	// Kết thúc render (present to screen)
	Renderer::GetInstance()->EndRender();
}

// Window procedure - xử lý tin nhắn từ Windows
// Được gọi bởi Windows khi có sự kiện cửa sổ
LRESULT CALLBACK WinApp::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		// Cửa sổ bị destroy -> post quit message để thoát vòng lặp game
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		// Phím được nhấn xuống
		GameManager::GetInstance()->OnKeyDown((int)wParam);
		break;
	case WM_KEYUP:
		// Phím được thả ra
		GameManager::GetInstance()->OnKeyUp((int)wParam);
		break;
	}
	// Để Windows xử lý các message khác
	return DefWindowProc(hWnd, message, wParam, lParam);
}