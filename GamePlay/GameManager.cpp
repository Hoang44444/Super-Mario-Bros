#include <iostream>
#include <fstream>
#include <sstream>

#include "GameManager.h"
#include "PlayScene.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"
#include "Renderer.h"

GameManager* GameManager::__instance = NULL;

GameManager::GameManager()
{
	current_scene = -1;
	next_scene = -1;
	hWnd = NULL;
	screenWidth = 0;
	screenHeight = 0;
	key_handler = NULL;
}

GameManager* GameManager::GetInstance()
{
	if (__instance == NULL) __instance = new GameManager();
	return __instance;
}

void GameManager::Init(HWND hWnd, HINSTANCE hInstance)
{
	this->hWnd = hWnd;
	Renderer::GetInstance()->Init(hWnd, hInstance);
}

void GameManager::ProcessKeyboard()
{
	if (key_handler != NULL)
	{
		BYTE states[256];
		if (GetKeyboardState(states))
		{
			key_handler->KeyState(states);
		}
	}
}

void GameManager::OnKeyDown(int KeyCode)
{
	if (key_handler != NULL)
		key_handler->OnKeyDown(KeyCode);
}

void GameManager::OnKeyUp(int KeyCode)
{
	if (key_handler != NULL)
		key_handler->OnKeyUp(KeyCode);
}

void GameManager::LoadSettings(LPCWSTR gameFile)
{
	DebugOut(L"[INFO] Start loading game settings from : %s \n", gameFile);

	ifstream f;
	f.open(gameFile);

	if (!f.is_open())
	{
		DebugOut(L"[ERROR] Failed to open game file: %s\n", gameFile);
		return;
	}

	int section = -1;

	char str[1024];
	while (f.getline(str, 1024))
	{
		string line(str);
		if (line.empty() || line[0] == '#') continue;

		if (line == "[SETTINGS]") { section = GAME_SECTION_SETTINGS; continue; }
		if (line[0] == '[') { section = -1; continue; }

		if (section == GAME_SECTION_SETTINGS)
			_ParseSection_SETTINGS(line);
	}
	f.close();
}

/*
	Load master configuration file
*/
void GameManager::Load(LPCWSTR gameFile)
{
	DebugOut(L"[INFO] Start loading game from : %s \n", gameFile);

	ifstream f;
	f.open(gameFile);

	if (!f.is_open())
	{
		DebugOut(L"[ERROR] Failed to open game file: %s\n", gameFile);
		return;
	}

	int section = -1;

	char str[1024];
	while (f.getline(str, 1024))
	{
		string line(str);
		if (line.empty() || line[0] == '#') continue;

		if (line == "[SETTINGS]") { section = GAME_SECTION_SETTINGS; continue; }
		if (line == "[SCENES]") { section = GAME_SECTION_SCENES; continue; }
		if (line == "[TEXTURES]") { section = GAME_SECTION_TEXTURES; continue; }

		switch (section)
		{
		case GAME_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GAME_SECTION_SCENES: _ParseSection_SCENES(line); break;
		case GAME_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Done loading game from : %s \n", gameFile);

	SwitchScene();
}

// Better parser for GameManager::Load
void GameManager::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens;
	stringstream ss(line);
	string token;
	while (ss >> token) tokens.push_back(token);

	if (tokens.size() < 2) return;

	if (tokens[0] == "start")
		next_scene = atoi(tokens[1].c_str());
	else if (tokens[0] == "screen_width")
		screenWidth = atoi(tokens[1].c_str());
	else if (tokens[0] == "screen_height")
		screenHeight = atoi(tokens[1].c_str());
}

void GameManager::_ParseSection_SCENES(string line)
{
	vector<string> tokens;
	stringstream ss(line);
	string token;
	while (ss >> token) tokens.push_back(token);

	if (tokens.size() < 2) return;

	int id = atoi(tokens[0].c_str());
	string path = tokens[1];
	wstring wpath = wstring(path.begin(), path.end());

	scenes[id] = new PlayScene(id, wpath.c_str());
}

void GameManager::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens;
	stringstream ss(line);
	string token;
	while (ss >> token) tokens.push_back(token);

	if (tokens.size() < 2) return;

	int id = atoi(tokens[0].c_str());
	string path = tokens[1];
	wstring wpath = wstring(path.begin(), path.end());

	TextureManager::GetInstance()->Add(id, wpath.c_str());
}

void GameManager::SwitchScene()
{
	if (next_scene < 0 || next_scene == current_scene) return;

	DebugOut(L"[INFO] Switching to scene %d\n", next_scene);

	if (current_scene != -1)
		scenes[current_scene]->Unload();

	SpriteManager::GetInstance()->Clear();
	AnimationManager::GetInstance()->Clear();

	current_scene = next_scene;
	LPSCENE s = scenes[current_scene];
	GameManager::GetInstance()->SetKeyHandler(s->GetKeyEventHandler());
	s->Load();
}

void GameManager::Update(DWORD dt)
{
	if (current_scene != -1)
		scenes[current_scene]->Update(dt);
}

void GameManager::Render()
{
	if (current_scene != -1)
		scenes[current_scene]->Render();
}

GameManager::~GameManager()
{
}
