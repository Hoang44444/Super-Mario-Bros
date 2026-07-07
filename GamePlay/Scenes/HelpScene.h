#pragma once

#include "Scene.h"
#include "Texture.h"

// HelpScene - màn hình help/settings (điều khiển âm thanh)
// Cho phép người chơi điều chỉnh volume và xem hướng dẫn điều khiển
class HelpScene : public Scene
{
private:
	LPTEXTURE background;  // Hình nền
	ID3DX10Font* font;  // Font để vẽ text
	int selectedOption;  // Lựa chọn hiện tại (0=master, 1=music, 2=sfx, 3=back)

	void LoadBackgroundPath();  // Load đường dẫn background từ file
	void CreateFontIfNeeded();  // Tạo font nếu chưa có
	void DrawBackground();  // Vẽ background
	void DrawTextLine(const wchar_t* text, int x, int y, int width, int height, D3DXCOLOR color, DWORD format);  // Vẽ text
	void DrawVolumeOption(const wchar_t* label, int value, int x, int y, bool selected);  // Vẽ option volume
	void AdjustSelectedVolume(int delta);  // Điều chỉnh volume của option được chọn
	void ApplyVolumes();  // Áp dụng volume settings cho SoundManager

public:
	HelpScene(int id, LPCWSTR filePath);
	~HelpScene();

	void Load() override;
	void Unload() override;
	void Update(DWORD dt) override;
	void Render() override;
	void AddObject(LPGAMEOBJECT obj) override;

	void MoveSelection(int delta);  // Di chuyển lựa chọn (up/down)
	void AdjustSelection(int delta);  // Điều chỉnh giá trị (left/right)
	void ConfirmSelection();  // Xác nhận lựa chọn
	void BackToMenu();  // Quay về menu chính
};
