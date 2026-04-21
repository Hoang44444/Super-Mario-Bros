#include "SpriteManager.h"
#include "debug.h"

SpriteManager* SpriteManager::__instance = NULL;

SpriteManager* SpriteManager::GetInstance()
{
	if (__instance == NULL) __instance = new SpriteManager();
	return __instance;
}

void SpriteManager::Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	LPSPRITE s = new Sprite(id, left, top, right, bottom, tex);
	sprites[id] = s;

	DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, left, top, right, bottom);
}

LPSPRITE SpriteManager::Get(int id)
{
	LPSPRITE s = sprites[id];
	if (s == NULL)
		DebugOut(L"[ERROR] Sprite ID %d not found\n", id);
	return s;
}

void SpriteManager::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}
