#include "AnimationManager.h"
#include "debug.h"

// Khởi tạo instance singleton tĩnh
AnimationManager* AnimationManager::__instance = NULL;

AnimationManager* AnimationManager::GetInstance()
{
	// Lấy instance singleton - tạo mới nếu chưa tồn tại
	if (__instance == NULL) __instance = new AnimationManager();
	return __instance;
}

void AnimationManager::Add(int id, LPANIMATION ani)
{
	// Lưu animation với ID cho trước
	animations[id] = ani;
}

LPANIMATION AnimationManager::Get(int id)
{
	// Lấy animation theo ID
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Animation ID %d not found\n", id);
	return ani;
}

void AnimationManager::Clear()
{
	// Xóa tất cả animation để giải phóng bộ nhớ
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}
