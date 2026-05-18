#include "Background.h"
#include "AnimationManager.h"
#include "GameManager.h"
#include "../Resource/AssetID.h"

void Background::Render()
{
    LPANIMATION ani = AnimationManager::GetInstance()->Get(WorldRender());
    if (ani != nullptr)
        ani->Render(x, y, z);
}

int Background::WorldRender()
{
	int currentScene = GameManager::GetInstance()->GetCurrentSceneID();

	if (currentScene == SCENE::WORLD_1_1)
		return ANIMATION::BACKGROUND_LEVEL1_1;
	if (currentScene == SCENE::WORLD_1_2)
		return ANIMATION::BACKGROUND_LEVEL1_2;
	if (currentScene == SCENE::WORLD_1_1)
		return ANIMATION::BACKGROUND_LEVEL1_1;
	if (currentScene == SCENE::WORLD_1_2)
		return ANIMATION::BACKGROUND_LEVEL1_2;
	if (currentScene == SCENE::WORLD_1_1)
		return ANIMATION::BACKGROUND_LEVEL1_1;
	if (currentScene == SCENE::WORLD_1_2)
		return ANIMATION::BACKGROUND_LEVEL1_2;
	if (currentScene == SCENE::WORLD_1_1)
		return ANIMATION::BACKGROUND_LEVEL1_1;
	if (currentScene == SCENE::WORLD_1_2)
		return ANIMATION::BACKGROUND_LEVEL1_2;
	if (currentScene == SCENE::WORLD_1_1)
		return ANIMATION::BACKGROUND_LEVEL1_1;
	if (currentScene == SCENE::WORLD_1_2)
		return ANIMATION::BACKGROUND_LEVEL1_2;
	if (currentScene == SCENE::WORLD_1_1)
		return ANIMATION::BACKGROUND_LEVEL1_1;
	if (currentScene == SCENE::WORLD_1_2)
		return ANIMATION::BACKGROUND_LEVEL1_2;
	if (currentScene == SCENE::WORLD_1_1)
		return ANIMATION::BACKGROUND_LEVEL1_1;
	if (currentScene == SCENE::WORLD_1_2)
		return ANIMATION::BACKGROUND_LEVEL1_2;
}
