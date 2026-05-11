#include <iostream>
#include <fstream>
#include <sstream>

#include "GameManager.h"
#include "PlayScene.h"
#include "TransitionScene.h"
#include "MenuScene.h"

#include "TextureManager.h"
#include "SpriteManager.h"
#include "AnimationManager.h"

#include "../Resource/AssetID.h"

#include "debug.h"
#include "Renderer.h"

using namespace std;

GameManager* GameManager::__instance = NULL;

static string Trim(string s)
{
    while (!s.empty() &&
        (s.back() == '\r' ||
            s.back() == '\n' ||
            s.back() == ' '))
    {
        s.pop_back();
    }

    while (!s.empty() && s.front() == ' ')
    {
        s.erase(s.begin());
    }

    return s;
}

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
    if (__instance == NULL)
        __instance = new GameManager();

    return __instance;
}

void GameManager::Init(HWND hWnd, HINSTANCE hInstance)
{
    this->hWnd = hWnd;

    Renderer::GetInstance()->Init(hWnd, hInstance);
}

void GameManager::SetKeyHandler(LPKEYEVENTHANDLER handler)
{
    this->key_handler = handler;
}

void GameManager::InitiateSwitchScene(int scene_id)
{
    next_scene = scene_id;
}
void GameManager::ProcessKeyboard()
{
    if (key_handler == NULL)
        return;

    BYTE states[256];

    if (GetKeyboardState(states))
    {
        key_handler->KeyState(states);
    }
}

void GameManager::OnKeyDown(int KeyCode)
{
    if (key_handler)
    {
        key_handler->OnKeyDown(KeyCode);
    }
}

void GameManager::OnKeyUp(int KeyCode)
{
    if (key_handler)
    {
        key_handler->OnKeyUp(KeyCode);
    }
}

void GameManager::_ParseSection_SETTINGS(string line)
{
    size_t commentPos = line.find('#');

    if (commentPos != string::npos)
    {
        line = line.substr(0, commentPos);
    }

    vector<string> tokens;

    stringstream ss(line);

    string token;

    while (ss >> token)
    {
        tokens.push_back(token);
    }

    if (tokens.size() < 2)
        return;

    if (tokens[0] == "start")
    {
        next_scene = atoi(tokens[1].c_str());
    }
    else if (tokens[0] == "screen_width")
    {
        screenWidth = atoi(tokens[1].c_str());
    }
    else if (tokens[0] == "screen_height")
    {
        screenHeight = atoi(tokens[1].c_str());
    }
}

void GameManager::_ParseSection_SCENES(string line)
{
    size_t commentPos = line.find('#');

    if (commentPos != string::npos)
    {
        line = line.substr(0, commentPos);
    }

    vector<string> tokens;

    stringstream ss(line);

    string token;

    while (ss >> token)
    {
        tokens.push_back(token);
    }

    if (tokens.size() < 2)
        return;

    int id = atoi(tokens[0].c_str());

    string path = tokens[1];

    wstring wpath(path.begin(), path.end());

    if (id == 0)
    {
        scenes[id] = new MenuScene(id, wpath.c_str());

        DebugOut(
            L"[INFO] MenuScene added: ID=%d Path=%s\n",
            id,
            wpath.c_str()
        );
    }
    else
    {
        scenes[id] = new PlayScene(id, wpath.c_str());

        DebugOut(
            L"[INFO] PlayScene added: ID=%d Path=%s\n",
            id,
            wpath.c_str()
        );
    }
}

void GameManager::_ParseSection_TEXTURES(string line)
{
    size_t commentPos = line.find('#');

    if (commentPos != string::npos)
    {
        line = line.substr(0, commentPos);
    }

    vector<string> tokens;

    stringstream ss(line);

    string token;

    while (ss >> token)
    {
        tokens.push_back(token);
    }

    if (tokens.size() < 2)
        return;

    int id = atoi(tokens[0].c_str());

    string path = tokens[1];

    wstring wpath(path.begin(), path.end());

    TextureManager::GetInstance()->Add(
        id,
        wpath.c_str()
    );
}

void GameManager::LoadSettings(LPCWSTR gameFile)
{
    DebugOut(
        L"[INFO] Loading settings from %s\n",
        gameFile
    );
}

void GameManager::Load(LPCWSTR gameFile)
{
    DebugOut(
        L"[INFO] Loading game file: %s\n",
        gameFile
    );

    ifstream f(gameFile);

    if (!f.is_open())
    {
        DebugOut(
            L"[ERROR] Cannot open game file!\n"
        );

	// Scene IDs 101, 102, 103 are transition screens.
	// Format in super-mario-bros.txt:
	//   101  transition_1to2  target_scene_id  tex_id  display_ms
	if (tokens.size() >= 5 && path == "transition")
	{
		int target = atoi(tokens[2].c_str());
		int tex_id = atoi(tokens[3].c_str());
		DWORD ms = (DWORD)atoi(tokens[4].c_str());
		scenes[id] = new TransitionScene(id, L"transition", target, tex_id, ms);
	}
	else
	{
		scenes[id] = new PlayScene(id, wpath.c_str());
	}
}

    int section = -1;

    char str[1024];

    while (f.getline(str, 1024))
    {
        string line = Trim(str);

        if (line.empty())
            continue;

        if (line[0] == '#')
            continue;

        if (line.find("[SETTINGS]") != string::npos)
        {
            section = GAME_SECTION_SETTINGS;
            continue;
        }

        if (line.find("[SCENES]") != string::npos)
        {
            section = GAME_SECTION_SCENES;
            continue;
        }

        if (line.find("[TEXTURES]") != string::npos)
        {
            section = GAME_SECTION_TEXTURES;
            continue;
        }

        switch (section)
        {
        case GAME_SECTION_SETTINGS:
            _ParseSection_SETTINGS(line);
            break;

        case GAME_SECTION_SCENES:
            _ParseSection_SCENES(line);
            break;

        case GAME_SECTION_TEXTURES:
            _ParseSection_TEXTURES(line);
            break;
        }
    }

    f.close();

    DebugOut(
        L"[INFO] Finished loading game\n"
    );

    SwitchScene();
}

void GameManager::SwitchScene()
{
    if (next_scene < 0)
        return;

    if (next_scene == current_scene)
        return;

    DebugOut(
        L"[INFO] Switching to scene %d\n",
        next_scene
    );

    if (current_scene != -1)
    {
        auto oldIt = scenes.find(current_scene);

        if (oldIt != scenes.end() &&
            oldIt->second != nullptr)
        {
            oldIt->second->Unload();
        }
    }

    SpriteManager::GetInstance()->Clear();
    AnimationManager::GetInstance()->Clear();

    current_scene = next_scene;
    next_scene = -1;

	current_scene = next_scene;
	next_scene = -1;  // Reset so SwitchScene() does not fire again next frame
	LPSCENE s = scenes[current_scene];
	GameManager::GetInstance()->SetKeyHandler(s->GetKeyEventHandler());
	s->Load();
}

void GameManager::Update(DWORD dt)
{
    if (next_scene != -1)
    {
        SwitchScene();
    }

    if (current_scene == -1)
        return;

    auto it = scenes.find(current_scene);

    if (it == scenes.end())
        return;

    LPSCENE scene = it->second;

    if (scene == nullptr)
        return;

    scene->Update(dt);
}

void GameManager::Render()
{
    if (current_scene == -1)
        return;

    auto it = scenes.find(current_scene);

    if (it == scenes.end())
        return;

    LPSCENE scene = it->second;

    if (scene == nullptr)
        return;

    scene->Render();
}

GameManager::~GameManager()
{
}