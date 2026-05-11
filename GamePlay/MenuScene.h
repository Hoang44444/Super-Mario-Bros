#pragma once

#include "Scene.h"
#include "GameObject.h"
#include "debug.h"

class MenuScene : public Scene
{
protected:

    int selectedOption = 0;

    void _ParseSection_SPRITES(std::string line);
    void _ParseSection_ANIMATIONS(std::string line);
    void _ParseSection_ASSETS(std::string line);

public:

    MenuScene(int id, LPCWSTR filePath)
        : Scene(id, filePath)
    {
    }

    virtual void Load() override;
    virtual void Update(DWORD dt) override;
    virtual void Render() override;
    virtual void Unload() override;

    virtual void AddObject(LPGAMEOBJECT obj) override {}

    void SetOption(int opt)
    {
        selectedOption = opt;
    }

    int GetOption()
    {
        return selectedOption;
    }
};

class MenuKeyHandler : public SceneKeyHandler
{
public:

    MenuKeyHandler(LPSCENE s)
        : SceneKeyHandler(s)
    {
    }

    virtual void KeyState(BYTE* states) override {}

    virtual void OnKeyDown(int KeyCode) override;

    virtual void OnKeyUp(int KeyCode) override {}
};