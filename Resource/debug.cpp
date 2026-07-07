#include "debug.h"
#include <Windows.h>

// Handle cửa sổ game - dùng bởi DebugOutTitle để set title
HWND _hwnd = NULL;

void DebugOut(const wchar_t* fmt, ...)
{
	// Format string với variable arguments
	va_list argp;
	va_start(argp, fmt);
	wchar_t dbg_out[4096];
	vswprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	
	// Output ra Visual Studio Output window
	// Có thể xem ở: View -> Output (hoặc Alt+2 trong VS)
	OutputDebugString(dbg_out);
}

void DebugOutTitle(const wchar_t* fmt, ...)
{
	// Format string
	wchar_t s[1024];
	VA_PRINTS(s);
	
	// Set text cho tiêu đề cửa sổ game
	// Phải gọi SetDebugWindow() trước để set _hwnd
	SetWindowText(_hwnd, s);
}

void SetDebugWindow(HWND hwnd)
{
	// Lưu handle cửa sổ để DebugOutTitle có thể dùng
	// Gọi hàm này trong WinMain hoặc khi khởi tạo cửa sổ
	_hwnd = hwnd;
}