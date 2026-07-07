#pragma once

#include "Scene.h"
#include "Texture.h"

// LevelScene - màn hình chọn level (1-1, 1-2, 1-3, 1-4)
// Cho phép người chơi chọn level để chơi
class LevelScene : public Scene
{
private:
	LPTEXTURE background;  // Hình nền
	ID3DX10Font* font;  // Font cho text level
	ID3DX10Font* titleFont;  // Font cho title
	int selectedLevel;  // Level được chọn (0-3)

	void LoadBackgroundPath();  // Load đường dẫn background từ file
	void CreateFontIfNeeded();  // Tạo font nếu chưa có
	void DrawBackground();  // Vẽ background
	void DrawTextLine(const wchar_t* text, int x, int y, int width, int height, D3DXCOLOR color, DWORD format);  // Vẽ text
	void DrawTitleText(const wchar_t* text, int x, int y, int width, int height, D3DXCOLOR color, DWORD format);  // Vẽ title (font lớn)
	void DrawCenteredText(const wchar_t* text, int y, int height, D3DXCOLOR color);  // Vẽ text căn giữa

public:
	LevelScene(int id, LPCWSTR filePath);
	~LevelScene();

	void Load() override;
	void Unload() override;
	void Update(DWORD dt) override;
	void Render() override;
	void AddObject(LPGAMEOBJECT obj) override;

	void MoveSelection(int delta);  // Di chuyển lựa chọn level (left/right)
	void ConfirmSelection();  // Xác nhận và bắt đầu level
	void BackToMenu();  // Quay về menu chính
};
