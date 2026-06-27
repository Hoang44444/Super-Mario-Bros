#pragma once

#include "Scene.h"
#include "Texture.h"

class LevelScene : public Scene
{
private:
	LPTEXTURE background;
	ID3DX10Font* font;
	ID3DX10Font* titleFont;
	int selectedLevel;

	void LoadBackgroundPath();
	void CreateFontIfNeeded();
	void DrawBackground();
	void DrawTextLine(const wchar_t* text, int x, int y, int width, int height, D3DXCOLOR color, DWORD format);
	void DrawTitleText(const wchar_t* text, int x, int y, int width, int height, D3DXCOLOR color, DWORD format);
	void DrawCenteredText(const wchar_t* text, int y, int height, D3DXCOLOR color);

public:
	LevelScene(int id, LPCWSTR filePath);
	~LevelScene();

	void Load() override;
	void Unload() override;
	void Update(DWORD dt) override;
	void Render() override;
	void AddObject(LPGAMEOBJECT obj) override;

	void MoveSelection(int delta);
	void ConfirmSelection();
	void BackToMenu();
};
