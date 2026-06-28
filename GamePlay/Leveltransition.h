#pragma once
#include <Windows.h>
#include <d3dx10.h>

// Reproduces the classic Super Mario Bros. level-intro card:
// a solid black screen showing "WORLD x-x" and the player's remaining
// lives, held for a fixed duration before gameplay actually begins.
class LevelTransition
{
private:
	static const DWORD DURATION_MS = 1200; // ~ same length as the NES card

	ID3DX10Font* font = nullptr;
	DWORD elapsed = 0;
	int world = 1;
	int stage = 1;
	int lives = 3;

	void CreateFontIfNeeded();
	void DrawCentered(const wchar_t* text, int y, int fontSize);

public:
	~LevelTransition();

	// Begin showing the card for the given world-stage / lives count.
	void Start(int world, int stage, int lives);

	// Advance the timer. Returns true once the card has been shown
	// long enough and the real scene switch should happen.
	bool Update(DWORD dt);

	void Render();
};