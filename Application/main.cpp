#include "WinApp.h"
#include "Renderer.h"

// Entry point của ứng dụng Windows
// WinMain là hàm main cho Windows applications (khác với main() của console)
// hInstance: handle của instance hiện tại
// hPrevInstance: không dùng trong Win32 hiện đại (luôn NULL)
// lpCmdLine: command line arguments (string)
// nCmdShow: flag cho biết cách hiển thị cửa sổ (minimized, maximized, v.v.)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// Tạo instance của WinApp
	WinApp myApp;
	
	// Khởi tạo cửa sổ và các hệ thống game
	// 800x600 là kích thước mặc định (có thể bị override bởi file config)
	if (myApp.Initialize(hInstance, 800, 600)) {
		// Khởi tạo thành công -> chạy vòng lặp game
		return myApp.Run();
	}

	// Khởi tạo thất bại
	return 0;
}