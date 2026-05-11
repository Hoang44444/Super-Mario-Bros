#pragma once

#include <Windows.h>
#include <unordered_map>
#include <string>
#include <vector>

#include "Scene.h"

class GameManager
{
private:
    static GameManager* __instance;

    HWND hWnd;

    std::unordered_map<int, LPSCENE> scenes;

    int current_scene;
    int next_scene;

    LPKEYEVENTHANDLER key_handler;

    int screenWidth;
    int screenHeight;

    void _ParseSection_SETTINGS(std::string line);
    void _ParseSection_SCENES(std::string line);
    void _ParseSection_TEXTURES(std::string line);

public:
    GameManager();

    void Init(HWND hWnd, HINSTANCE hInstance);

    void LoadSettings(LPCWSTR gameFile);
    void Load(LPCWSTR gameFile);

    void Update(DWORD dt);
    void Render();

    void ProcessKeyboard();
    void OnKeyDown(int KeyCode);
    void OnKeyUp(int KeyCode);

    void SwitchScene();
    void InitiateSwitchScene(int scene_id);

    void SetKeyHandler(LPKEYEVENTHANDLER handler);

    int GetScreenWidth() { return screenWidth; }
    int GetScreenHeight() { return screenHeight; }

    LPSCENE GetCurrentScene();

    static GameManager* GetInstance();

    ~GameManager();
};