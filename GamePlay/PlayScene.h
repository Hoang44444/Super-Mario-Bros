#pragma once
#include "Scene.h"
class PlayScene : public Scene
{
public:
	PlayScene(int id, LPCWSTR filePath) : Scene(id, filePath) {}
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
};

