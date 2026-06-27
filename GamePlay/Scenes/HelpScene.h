#pragma once

#include "Scene.h"
#include "Texture.h"

class HelpScene : public Scene
{
private:
	LPTEXTURE background;
	ID3DX10Font* font;
	int selectedOption;

	void LoadBackgroundPath();
	void CreateFontIfNeeded();
	void DrawBackground();
	void DrawTextLine(const wchar_t* text, int x, int y, int width, int height, D3DXCOLOR color, DWORD format);
	void DrawVolumeOption(const wchar_t* label, int value, int x, int y, bool selected);
	void AdjustSelectedVolume(int delta);
	void ApplyVolumes();

public:
	HelpScene(int id, LPCWSTR filePath);
	~HelpScene();

	void Load() override;
	void Unload() override;
	void Update(DWORD dt) override;
	void Render() override;
	void AddObject(LPGAMEOBJECT obj) override;

	void MoveSelection(int delta);
	void AdjustSelection(int delta);
	void ConfirmSelection();
	void BackToMenu();
};
