
#pragma once
#include "Scene.h"
#include "GameObject.h"
#include <vector>
#include "debug.h"

class MenuOptions;   // menu-only object; full definition pulled in by PlayScene.cpp
class HUD;
class MenuHUD;

class PlayScene : public Scene
{
	LPGAMEOBJECT player;
	std::vector<LPGAMEOBJECT> objects;
	MenuOptions* menuOptions = nullptr;   // the menu's options object (null on non-menu scenes)
	HUD* hud = nullptr;
	MenuHUD* menuHUD = nullptr;           // menu HUD for text-based menu (null on non-menu scenes)
	float fixedCameraY;
	int mapHeight = 0;            // map height (px); used to detect Mario falling off the bottom
	ULONGLONG marioDieStart = 0;  // tick when Mario entered the DIE state (0 = not dying)
	ULONGLONG sceneStart = 0;     // tick when the scene started loading

	void OnMarioDeath();          // lose a life -> death screen, or game over -> menu

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void LoadAssets(LPCWSTR assetFile);

public:
	PlayScene(int id, LPCWSTR filePath) : Scene(id, filePath) {
		player = NULL;
		hud = NULL;
		fixedCameraY = 0;
	}
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void AddObject(LPGAMEOBJECT obj) {
		obj->SetScene(this);
		objects.push_back(obj);
		DebugOut(L"[SCENE] Added new object. Total objects: %d\n", objects.size());
	}

	LPGAMEOBJECT GetPlayer() { return player; }
	std::vector<LPGAMEOBJECT>& GetObjects() { return objects; }
	MenuOptions* GetMenuOptions() { return menuOptions; }
	MenuHUD* GetMenuHUD() { return menuHUD; }

	// Build an item object by its OBJECT type id (returns nullptr if not an item).
	LPGAMEOBJECT CreateItem(int type, float x, float y, float z);
};

