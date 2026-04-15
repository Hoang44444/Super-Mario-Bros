#pragma once
#include <unordered_map>
#include "Scene.h"

using namespace std;

class SceneManager
{
	static SceneManager* __instance;
	unordered_map<int, LPSCENE> scenes;
	int current_scene;

public:
	static SceneManager* GetInstance();

	void Add(int id, LPSCENE scene);
	void SwitchScene(int id);

	LPSCENE GetActiveScene() { return scenes[current_scene]; }

	void Load(int id);
	void Unload();
	void Update(DWORD dt);
	void Render();
};
