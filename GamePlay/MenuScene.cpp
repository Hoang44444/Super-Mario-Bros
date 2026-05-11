#include "MenuScene.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "SpriteManager.h"
#include "AnimationManager.h"
#include "Renderer.h"
#include "GameManager.h"
#include "TextureManager.h"
#include "debug.h"

using namespace std;

#define SCENE_SECTION_ASSETS        1
#define SCENE_SECTION_SPRITES       2
#define SCENE_SECTION_ANIMATIONS    3

/*
    =========================================================
                        PARSE SECTION
    =========================================================
*/

void MenuScene::_ParseSection_ASSETS(string line)
{
    // Nếu sau này cần load asset riêng cho menu
    // thì xử lý ở đây
}

void MenuScene::_ParseSection_SPRITES(string line)
{
    vector<string> tokens;

    stringstream ss(line);
    string token;

    while (ss >> token)
        tokens.push_back(token);

    if (tokens.size() < 6)
    {
        DebugOut(L"[ERROR] Invalid sprite line: %S\n", line.c_str());
        return;
    }

    int id = atoi(tokens[0].c_str());
    int left = atoi(tokens[1].c_str());
    int top = atoi(tokens[2].c_str());
    int right = atoi(tokens[3].c_str());
    int bottom = atoi(tokens[4].c_str());
    int texID = atoi(tokens[5].c_str());

    LPTEXTURE tex = TextureManager::GetInstance()->Get(texID);

    if (tex == NULL)
    {
        DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
        return;
    }

    SpriteManager::GetInstance()->Add(
        id,
        left,
        top,
        right,
        bottom,
        tex
    );

    DebugOut(L"[INFO] Sprite %d loaded\n", id);
}

void MenuScene::_ParseSection_ANIMATIONS(string line)
{
    // Chưa dùng animation cho menu
}

/*
    =========================================================
                            LOAD
    =========================================================
*/

void MenuScene::Load()
{
    DebugOut(L"[INFO] Start loading MenuScene\n");

    ifstream f;
    f.open(sceneFilePath);

    if (!f.is_open())
    {
        DebugOut(
            L"[ERROR] Cannot open scene file: %s\n",
            sceneFilePath
        );

        return;
    }

    int section = -1;

    char str[1024];

    while (f.getline(str, 1024))
    {
        string line(str);

        if (line.empty() || line[0] == '#')
            continue;

        if (line == "[ASSETS]")
        {
            section = SCENE_SECTION_ASSETS;
            continue;
        }

        if (line == "[SPRITES]")
        {
            section = SCENE_SECTION_SPRITES;
            continue;
        }

        if (line == "[ANIMATIONS]")
        {
            section = SCENE_SECTION_ANIMATIONS;
            continue;
        }

        switch (section)
        {
        case SCENE_SECTION_ASSETS:
            _ParseSection_ASSETS(line);
            break;

        case SCENE_SECTION_SPRITES:
            _ParseSection_SPRITES(line);
            break;

        case SCENE_SECTION_ANIMATIONS:
            _ParseSection_ANIMATIONS(line);
            break;
        }
    }

    f.close();

    /*
        Tạo key handler
    */
    key_handler = new MenuKeyHandler(this);

    DebugOut(L"[INFO] MenuScene loaded successfully\n");
}

/*
    =========================================================
                            UPDATE
    =========================================================
*/

void MenuScene::Update(DWORD dt)
{
    // Menu hiện tại chưa cần update gì
}

/*
    =========================================================
                            RENDER
    =========================================================
*/

void MenuScene::Render()
{
    auto renderer = Renderer::GetInstance();
    auto sprites = SpriteManager::GetInstance();

    renderer->BeginRender();

    /*
        =====================================================
                        DRAW MENU BACKGROUND
        =====================================================
    */

    LPSPRITE title = sprites->Get(10001);
    if (title == nullptr)
    {
        DebugOut(L"[ERROR] title sprite NULL\n");
    }
    else
    {
        DebugOut(L"[INFO] title sprite OK\n");
    }
    if (title)
    {
        RECT r = title->GetRect();

        renderer->Draw(
            100.0f,
            10.0f,
            title->GetTexture(),
            &r
        );
    }
    else
    {
        DebugOut(L"[ERROR] Missing sprite 10001\n");
    }

    /*
        =====================================================
                        DRAW CURSOR
        =====================================================
    */

    LPSPRITE cursor = sprites->Get(10002);

    if (cursor)
    {
        RECT r = cursor->GetRect();

        float cursorX = 80.0f;

        float cursorY =
            (selectedOption == 0)
            ? 145.0f
            : 160.0f;

        renderer->Draw(
            cursorX,
            cursorY,
            cursor->GetTexture(),
            &r
        );
    }
    else
    {
        DebugOut(L"[ERROR] Missing sprite 10002\n");
    }

    renderer->EndRender();
}

/*
    =========================================================
                            UNLOAD
    =========================================================
*/

void MenuScene::Unload()
{
    DebugOut(L"[INFO] Unloading MenuScene\n");
}

/*
    =========================================================
                        MENU KEY HANDLER
    =========================================================
*/

void MenuKeyHandler::OnKeyDown(int KeyCode)
{
    MenuScene* menu = (MenuScene*)scence;

    switch (KeyCode)
    {
    case VK_UP:

        menu->SetOption(0);

        DebugOut(L"[INFO] Selected: 1 PLAYER\n");

        break;

    case VK_DOWN:

        menu->SetOption(1);

        DebugOut(L"[INFO] Selected: 2 PLAYER\n");

        break;

    case VK_RETURN:

        DebugOut(L"[INFO] Start Game\n");

        /*
            KHÔNG gọi SwitchScene trực tiếp
            chỉ set next_scene
        */

        GameManager::GetInstance()->InitiateSwitchScene(1);

        break;
    }
}