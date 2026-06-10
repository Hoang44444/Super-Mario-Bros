#pragma once
#include "Scene.h"

class MenuScene : public Scene
{
public:
	MenuScene(int id, LPCWSTR filePath) : Scene(id, filePath) {}
	virtual void Load() override {}
	virtual void Unload() override {}
	virtual void Update(DWORD dt) override {}
	virtual void Render() override {}
	virtual void AddObject(LPGAMEOBJECT obj) override {}
	virtual ~MenuScene() {}
};