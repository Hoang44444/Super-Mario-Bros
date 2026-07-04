#pragma once

#include <Windows.h>
#include <d3dx10.h>

// Quản lý giao diện Menu chính (START, LEVEL, HELP, QUIT)
// Hiển thị con trỏ chọn (icon nấm) và Mario trang trí
class MenuHUD
{
private:
	ID3DX10Font* font;           // Font vẽ text menu
	int selectedOption;           // Tùy chọn đang chọn (0=START, 1=LEVEL, 2=HELP, 3=QUIT)
	float cursorX, cursorY;       // Vị trí con trỏ chọn (icon nấm)

	// Biến bố cục menu (tính trong Update, dùng trong Render)
	float menuStartX;             // Tọa độ X bắt đầu menu
	float menuStartY;             // Tọa độ Y bắt đầu menu
	float menuSpacing;            // Khoảng cách giữa các dòng
	float fontHeight;             // Chiều cao font

	void CreateFontIfNeeded();
	void DrawTextLine(const wchar_t* text, int x, int y, int width, int height, D3DXCOLOR color);
	void DrawCursor();

public:
	MenuHUD();
	~MenuHUD();

	void Update();
	void Render();

	void MoveSelection(int delta);
	int GetSelectedOption() const { return selectedOption; }
	void ResetSelection() { selectedOption = 0; }
};
