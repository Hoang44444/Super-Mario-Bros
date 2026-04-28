#pragma once
#include <Windows.h>
#include <unordered_map>
#include <string>

#include "Scene.h"

class GameManager
{
	static GameManager* __instance;
	HWND hWnd;									// Window handle

	std::unordered_map<int, LPSCENE> scenes;
	int current_scene;
	int next_scene;

	LPKEYEVENTHANDLER key_handler;

	int screenWidth;
	int screenHeight;

	void _ParseSection_SETTINGS(std::string line);
	void _ParseSection_SCENES(std::string line);
	void _ParseSection_TEXTURES(std::string line);

public:
	GameManager();
	void Init(HWND hWnd, HINSTANCE hInstance);

	void SetKeyHandler(LPKEYEVENTHANDLER handler) { this->key_handler = handler; }
	void ProcessKeyboard();
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);

	void LoadSettings(LPCWSTR gameFile);
	void Load(LPCWSTR gameFile);

	int GetScreenWidth() { return screenWidth; }
	int GetScreenHeight() { return screenHeight; }

	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene();
	void InitiateSwitchScene(int scene_id) { next_scene = scene_id; }

	void Update(DWORD dt);
	void Render();

	static GameManager* GetInstance();

	~GameManager();
};
