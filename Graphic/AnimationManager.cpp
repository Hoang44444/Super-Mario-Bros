#include "AnimationManager.h"
#include "debug.h"

AnimationManager* AnimationManager::__instance = NULL;

AnimationManager* AnimationManager::GetInstance()
{
	if (__instance == NULL) __instance = new AnimationManager();
	return __instance;
}

void AnimationManager::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION AnimationManager::Get(int id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Animation ID %d not found\n", id);
	return ani;
}

void AnimationManager::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}
