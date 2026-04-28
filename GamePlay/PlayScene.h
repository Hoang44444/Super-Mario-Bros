
#pragma once
#include "Scene.h"
#include "GameObject.h"
#include <vector>

class PlayScene : public Scene
{
	LPGAMEOBJECT player;
	std::vector<LPGAMEOBJECT> objects;

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void LoadAssets(LPCWSTR assetFile);

public:
	PlayScene(int id, LPCWSTR filePath) : Scene(id, filePath) {
		player = NULL;
	}
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
};

