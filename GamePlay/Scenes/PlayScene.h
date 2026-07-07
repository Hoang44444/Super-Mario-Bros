#pragma once
#include "Scene.h"
#include "GameObject.h"
#include <vector>
#include "debug.h"

class MenuOptions;
class HUD;
class MenuHUD;

class PlayScene : public Scene
{
	LPGAMEOBJECT player = nullptr;
	std::vector<LPGAMEOBJECT> objects;
	MenuOptions* menuOptions = nullptr;
	HUD* hud;
	MenuHUD* menuHUD;
	float fixedCameraY = 0.0f;
	int mapHeight = 0;
	ULONGLONG marioDieStart = 0;
	ULONGLONG sceneStart = 0;
	bool windSfxPlaying = false;
	bool earthquakeSfxPlaying = false;

	bool stageClearActive = false;
	ULONGLONG stageClearStart = 0;
	int stageClearTime = 0;
	int stageClearDisplayTime = 0;

	LPGAMEOBJECT bossBowser = nullptr;

	LPGAMEOBJECT CreatePlayerObject(float x, float y, float z);
	LPGAMEOBJECT CreateStaticObject(int type, float x, float y, float z, vector<string>& tokens);
	LPGAMEOBJECT CreateDynamicObject(int type, float x, float y, float z, vector<string>& tokens);
	LPGAMEOBJECT CreateEnemyObject(int type, float x, float y, float z, vector<string>& tokens);
	void HandleIntroDeathSceneUpdate();
	void HandleGameOverSceneUpdate();
	void UpdateSoundEffects();
	void UpdateStageClearSequence();
	void HandleDeathDetection();
	void UpdateCameraAndEarthquake(DWORD dt);

	void OnMarioDeath();
	bool IsPlayerOnCastleBridge();
	void ActivateBossIfOnBridge();

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void LoadAssets(LPCWSTR assetFile);

public:
	PlayScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void AddObject(LPGAMEOBJECT obj) {
		obj->SetScene(this);
		objects.push_back(obj);
	}

	LPGAMEOBJECT GetPlayer() { return player; }
	std::vector<LPGAMEOBJECT>& GetObjects() { return objects; }
	MenuOptions* GetMenuOptions() { return menuOptions; }
	MenuHUD* GetMenuHUD() { return menuHUD; }

	LPGAMEOBJECT CreateItem(int type, float x, float y, float z);

	void StartStageClear();
};

