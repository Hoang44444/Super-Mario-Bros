#pragma once

#include <Windows.h>
#include <d3dx10.h>

class Mario;

class HUD
{
private:
	static const int START_TIME = 400;

	Mario* mario;
	int world;
	int stage;
	int remainingTime;
	DWORD elapsedMs;
	ID3DX10Font* font;
	bool warningBGMPlaying;
	bool stageClearOverride;

	void CreateFontIfNeeded();
	void DrawTextLine(const wchar_t* text, int x, int y, int width, int height);
	void DrawCenteredTextLine(const wchar_t* text, int y, int height);
	void RenderTextureRectAtScreen(int textureId, const RECT& rect, float screenX, float screenY, float z = 0.9f);
	void RenderCoinIcon(float screenX, float screenY);
	float GetMarioLivesStartX() const;
	void RenderMarioLivesIcon(int y);
	void DrawMarioLivesText(int lives, int y);

public:
	HUD(Mario* mario, int world = 1, int stage = 1);
	~HUD();

	void SetMario(Mario* mario) { this->mario = mario; }
	void ResetTimer();
	void SetRemainingTime(int time) { remainingTime = time; }
	void SetStageClearActive(bool active) { stageClearOverride = active; }
	void Update(DWORD dt);
	void Render();

	int GetRemainingTime() const { return remainingTime; }
};
