#pragma once
#include "Scene.h"
#include "GameObject.h"
#include <vector>
#include "debug.h"

class MenuOptions;   // menu-only object; full definition pulled in by PlayScene.cpp
class HUD;
class MenuHUD;

// PlayScene - scene chính cho gameplay (level 1-1, 1-2, 1-3, 1-4) và menu screens
// Quản lý tất cả objects, player, HUD, camera, và game logic
class PlayScene : public Scene
{
	LPGAMEOBJECT player = nullptr;  // Mario
	std::vector<LPGAMEOBJECT> objects;  // Tất cả objects trong scene
	MenuOptions* menuOptions = nullptr;   // Menu options object (null ở non-menu scenes)
	HUD* hud;  // HUD cho gameplay (score, coins, time, lives)
	MenuHUD* menuHUD;  // Menu HUD cho text-based menu (null ở non-menu scenes)
	float fixedCameraY = 0.0f;  // Y cố định cho camera (không follow theo Mario Y)
	int mapHeight = 0;  // Chiều cao map (px); dùng để detect Mario rơi xuống đáy
	ULONGLONG marioDieStart = 0;  // Tick khi Mario vào state DIE (0 = không chết)
	ULONGLONG sceneStart = 0;  // Tick khi scene bắt đầu load
	bool windSfxPlaying = false;  // Track nếu wind SFX đang chơi
	bool earthquakeSfxPlaying = false;  // Track nếu earthquake SFX đang chơi

	// Stage clear sequence state
	bool stageClearActive = false;  // Stage clear sequence đang chạy
	ULONGLONG stageClearStart = 0;  // Tick khi stage clear bắt đầu
	int stageClearTime = 0;  // Thời gian còn lại khi stage clear bắt đầu (dùng để tính điểm)
	int stageClearDisplayTime = 0;  // Thời gian hiển thị trên màn hình (countdown nhanh)

	// Boss màn 1-4: Bowser đứng yên tại chỗ, chỉ kích hoạt khi Mario bước lên cầu.
	LPGAMEOBJECT bossBowser = nullptr;

	// Helper functions for _ParseSection_OBJECTS
	LPGAMEOBJECT CreatePlayerObject(float x, float y, float z);  // Tạo Mario
	LPGAMEOBJECT CreateStaticObject(int type, float x, float y, float z, vector<string>& tokens);  // Tạo static objects
	LPGAMEOBJECT CreateDynamicObject(int type, float x, float y, float z, vector<string>& tokens);  // Tạo dynamic objects
	LPGAMEOBJECT CreateEnemyObject(int type, float x, float y, float z, vector<string>& tokens);  // Tạo enemies
	void HandleIntroDeathSceneUpdate();  // Xử lý intro/death scene timeout
	void HandleGameOverSceneUpdate();  // Xử lý game over scene timeout
	void UpdateSoundEffects();  // Xử lý wind và earthquake SFX
	void UpdateStageClearSequence();  // Xử lý stage clear countdown
	void HandleDeathDetection();  // Xử lý Mario death detection
	void UpdateCameraAndEarthquake(DWORD dt);  // Xử lý camera follow và earthquake

	void OnMarioDeath();  // Mất mạng -> death screen, hoặc game over -> menu
	bool IsPlayerOnCastleBridge();  // Mario có đang đứng trên cầu cuối màn không
	void ActivateBossIfOnBridge();  // Kích hoạt Bowser khi Mario lên cầu

	void _ParseSection_ASSETS(string line);  // Parse section [ASSETS] từ file scene
	void _ParseSection_OBJECTS(string line);  // Parse section [OBJECTS] từ file scene
	void _ParseSection_MAP(string line);  // Parse section [MAP] từ file scene

	void _ParseSection_SPRITES(string line);  // Parse section [SPRITES] từ file asset
	void _ParseSection_ANIMATIONS(string line);  // Parse section [ANIMATIONS] từ file asset

	void LoadAssets(LPCWSTR assetFile);  // Load assets từ file

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

	// Build item object theo OBJECT type id (trả nullptr nếu không phải item)
	LPGAMEOBJECT CreateItem(int type, float x, float y, float z);

	// Stage clear sequence (được gọi bởi SwitchScenePoint)
	void StartStageClear();
};

