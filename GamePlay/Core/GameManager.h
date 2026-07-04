#pragma once
#include <Windows.h>
#include <unordered_map>
#include <string>

#include "Scene.h"
#include "../../Resource/SoundManager.h"

// GameManager là singleton quản lý toàn bộ game
// Quản lý scene switching, input routing, game state, và main loop
// Cách dùng:
//   1. Gọi GameManager::GetInstance() để lấy singleton
//   2. Init() để khởi tạo các hệ thống (Renderer, SoundManager)
//   3. Load() để load game config từ file
//   4. Update()/Render() mỗi frame từ WinApp
class GameManager
{
	static GameManager* __instance;  // Instance singleton
	HWND hWnd;								// Window handle

	// Map lưu tất cả scene theo ID
	std::unordered_map<int, LPSCENE> scenes;
	int current_scene;              // ID của scene hiện tại
	int next_scene;                 // ID của scene sẽ chuyển tới
	int game_state;                 // Trạng thái game (MENU, PLAY, PAUSE, GAME_OVER)

	LPKEYEVENTHANDLER key_handler;  // Handler xử lý input keyboard

	int screenWidth;                // Chiều rộng màn hình
	int screenHeight;               // Chiều cao màn hình

	// Parser methods cho file config
	void _ParseSection_SETTINGS(std::string line);
	void _ParseSection_SCENES(std::string line);
	void _ParseSection_TEXTURES(std::string line);

public:
	GameManager();
	
	// Khởi tạo các hệ thống game (Renderer, SoundManager)
	void Init(HWND hWnd, HINSTANCE hInstance);

	// Set key handler cho scene hiện tại
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { this->key_handler = handler; }
	
	// Xử lý keyboard state (được gọi mỗi frame)
	void ProcessKeyboard();
	
	// Xử lý key down event
	void OnKeyDown(int KeyCode);
	
	// Xử lý key up event
	void OnKeyUp(int KeyCode);

	// Load settings (screen width/height) từ file config
	void LoadSettings(LPCWSTR gameFile);
	
	// Load toàn bộ game (scenes, textures) từ file config
	void Load(LPCWSTR gameFile);

	// Getters cho kích thước màn hình
	int GetScreenWidth() { return screenWidth; }
	int GetScreenHeight() { return screenHeight; }

	// Getters cho scene
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	int GetCurrentSceneID() { return current_scene; }

	// Chuyển scene (thực hiện chuyển đổi thực tế)
	void SwitchScene();
	
	// Khởi tạo chuyển scene (set next_scene, chuyển sẽ diễn ra trong Update)
	void InitiateSwitchScene(int scene_id) { next_scene = scene_id; }

	// Getters/Setters cho game state
	int  GetGameState() { return game_state; }
	void SetGameState(int s) { game_state = s; }

	// Update scene hiện tại (được gọi mỗi frame)
	void Update(DWORD dt);
	
	// Render scene hiện tại (được gọi mỗi frame)
	void Render();

	// Lấy instance singleton
	static GameManager* GetInstance();

	~GameManager();
};
