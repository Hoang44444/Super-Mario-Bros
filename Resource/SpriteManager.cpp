#include "SpriteManager.h"

SpriteManager* SpriteManager::__instance = NULL;

SpriteManager* SpriteManager::GetInstance()
{
    if (__instance == NULL)
        __instance = new SpriteManager();
    return __instance;
}

void SpriteManager::Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
    if (sprites.find(id) != sprites.end())
        return; // tránh trùng

    LPSPRITE s = new Sprite(id, left, top, right, bottom, tex);
    sprites[id] = s;
}

LPSPRITE SpriteManager::Get(int id)
{
    auto it = sprites.find(id);
    if (it == sprites.end())
        return NULL;

    return it->second;
}

