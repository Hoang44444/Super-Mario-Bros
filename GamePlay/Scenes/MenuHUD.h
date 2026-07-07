#pragma once

#include <Windows.h>
#include <d3dx10.h>

class MenuHUD
{
private:
	ID3DX10Font* font;
	int selectedOption;
	float cursorX, cursorY;

	float menuStartX;
	float menuStartY;
	float menuSpacing;
	float fontHeight;

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
