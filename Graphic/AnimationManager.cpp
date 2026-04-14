#include "AnimationManager.h"

void AnimationManager::Add(int id, LPANIMATION ani)
{
    if (animations.find(id) != animations.end())
        return;

    animations[id] = ani;
}

LPANIMATION AnimationManager::Get(int id)
{
    auto it = animations.find(id);
    if (it == animations.end())
        return NULL;

    return it->second;
}

void AnimationManager::Clear()
{
    for (auto& a : animations)
        delete a.second;

    animations.clear();
}
