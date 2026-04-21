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

	void _ParseSection_SETTINGS(std::string line);
	void _ParseSection_SCENES(std::string line);
	void _ParseSection_TEXTURES(std::string line);

public:
	GameManager();
	void Init(HWND hWnd, HINSTANCE hInstance);

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene();
	void InitiateSwitchScene(int scene_id) { next_scene = scene_id; }

	void Update(DWORD dt);
	void Render();

	static GameManager* GetInstance();

	~GameManager();
};
