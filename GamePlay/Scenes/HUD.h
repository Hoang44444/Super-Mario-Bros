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

	void CreateFontIfNeeded();
	void DrawTextLine(const wchar_t* text, int x, int y, int width, int height);

public:
	HUD(Mario* mario, int world = 1, int stage = 1);
	~HUD();

	void SetMario(Mario* mario) { this->mario = mario; }
	void ResetTimer();
	void Update(DWORD dt);
	void Render();

	int GetRemainingTime() const { return remainingTime; }
};
