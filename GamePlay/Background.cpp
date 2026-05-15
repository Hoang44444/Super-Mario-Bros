#include "Background.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"

void Background::Render()
{
    LPANIMATION ani = AnimationManager::GetInstance()->Get(ID_ANI_BACKGROUND_LEVEL1_1);
    if (ani != NULL)
        ani->Render(x, y);
}
