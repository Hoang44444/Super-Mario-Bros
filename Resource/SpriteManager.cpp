#include "SpriteManager.h"
#include "debug.h"

// Khởi tạo instance singleton tĩnh
SpriteManager* SpriteManager::__instance = NULL;

SpriteManager* SpriteManager::GetInstance()
{
	if (__instance == NULL) __instance = new SpriteManager();
	return __instance;
}

void SpriteManager::Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	// Tạo sprite mới với vùng cho trước trên texture
	LPSPRITE s = new Sprite(id, left, top, right, bottom, tex);
	sprites[id] = s;
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
	// Xóa tất cả đối tượng Sprite để giải phóng bộ nhớ
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}
