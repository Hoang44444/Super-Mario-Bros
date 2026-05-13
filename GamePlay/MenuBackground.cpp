#include "MenuBackground.h"
#include "AssetID.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "debug.h"

#include "Renderer.h"
#include "Camera.h"
#include "TextureManager.h"

MenuBackground::MenuBackground(float x, float y)
    : GameObject(x, y)
{
}

void MenuBackground::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    // Background does not need update
}

void MenuBackground::Render()
{
    LPTEXTURE tex = TextureManager::GetInstance()->Get(ID_TEX_MENUBACKGROUND);
    if (tex == NULL) return;

    Renderer::GetInstance()->DrawScaled(x, y, tex,
        (float)Renderer::INTERNAL_SCREEN_WIDTH,   // 282
        (float)Renderer::INTERNAL_SCREEN_HEIGHT); // 240
}

void MenuBackground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = t = r = b = 0;
}
