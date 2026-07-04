#pragma once

#include <windows.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>

// Module debug - cung cấp các hàm để output thông tin debug
// Dùng để log lỗi, thông tin trong quá trình phát triển
// Output hiển thị trong Visual Studio Output window

// Macro để convert string thường sang wide string (L"...")
// Dùng: _W("hello") -> L"hello"
#define _W(x)  __W(x)
#define __W(x)  L##x

// Macro helper để format string với variable arguments
// Dùng bên trong các hàm debug để xử lý fmt, ...
#define VA_PRINTS(s) {				\
		va_list argp;				\
		va_start(argp, fmt);		\
		vswprintf_s(s, fmt, argp);	\
		va_end(argp);				\
}

// Output thông tin debug ra Visual Studio Output window
// Cách dùng: DebugOut(L"Texture ID: %d not found\n", id);
void DebugOut(const wchar_t* fmt, ...);

// Output thông tin debug ra tiêu đề cửa sổ game
// Cách dùng: DebugOutTitle(L"FPS: %d", fps);
void DebugOutTitle(const wchar_t* fmt, ...);

// Set handle cửa sổ để DebugOutTitle có thể cập nhật tiêu đề
// Phải gọi trước khi dùng DebugOutTitle
// Cách dùng: SetDebugWindow(hwnd);
void SetDebugWindow(HWND hwnd);
