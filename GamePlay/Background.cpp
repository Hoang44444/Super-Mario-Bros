#include "Background.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"

void Background::Render()
{
    LPANIMATION ani = AnimationManager::GetInstance()->Get(ANIMATION::BACKGROUND_LEVEL1_1);
    if (ani != nullptr)
        ani->Render(x, y, z);
}
