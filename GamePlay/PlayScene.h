#pragma once
#include "Scene.h"
#include "GameObject.h"
#include <vector>

class PlayScene : public Scene
{
	std::vector<LPGAMEOBJECT> objects;
public:
	PlayScene(int id, LPCWSTR filePath) : Scene(id, filePath) {}
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
};

