#include "Background.h"
#include "AnimationManager.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "../Resource/AssetID.h"

void Background::Render()
{
    LPANIMATION ani = AnimationManager::GetInstance()->Get(WorldRender());
    if (ani == nullptr) return;

    int currentScene = GameManager::GetInstance()->GetCurrentSceneID();

    // Menu / control / end are single static full-screen images authored at a higher
    // resolution than the game's logical screen (~440x240). Stretch them to fill the
    // screen instead of drawing at native pixel size (which would show only a corner).
    if (currentScene == SCENE::MENU ||
        currentScene == SCENE::CONTROL ||
        currentScene == SCENE::END ||
        currentScene == SCENE::DEATH ||
        currentScene == SCENE::GAME_OVER ||
        currentScene == SCENE::INTRO)
    {
        Renderer* r = Renderer::GetInstance();
        float scale = r->GetGlobalScale();
        float logicalW = r->GetBackBufferWidth() / scale;
        float logicalH = r->GetBackBufferHeight() / scale;

        // Anchor to the camera so the image always covers the visible area.
        float camX = Camera::GetInstance()->GetX();
        float camY = Camera::GetInstance()->GetY();
        ani->RenderScaled(camX, camY, z, logicalW, logicalH);
        return;
    }

    ani->Render(x, y, z);
}

int Background::WorldRender()
{
	int currentScene = GameManager::GetInstance()->GetCurrentSceneID();

	if (currentScene == SCENE::WORLD_1_1)
		return ANIMATION::BACKGROUND_LEVEL1_1;
	if (currentScene == SCENE::WORLD_1_2)
		return ANIMATION::BACKGROUND_LEVEL1_2;
	if (currentScene == SCENE::WORLD_1_3)
		return ANIMATION::BACKGROUND_LEVEL1_3;
	if (currentScene == SCENE::WORLD_1_4)
		return ANIMATION::BACKGROUND_LEVEL1_4;

	if (currentScene == SCENE::MENU)
		return ANIMATION::BACKGROUND_MENU;
	if (currentScene == SCENE::CONTROL)
		return ANIMATION::BACKGROUND_CONTROL;
	if (currentScene == SCENE::END)
		return ANIMATION::BACKGROUND_END;
	if (currentScene == SCENE::DEATH)
		return ANIMATION::BACKGROUND_DEATH;
	if (currentScene == SCENE::GAME_OVER)
		return ANIMATION::BACKGROUND_GAMEOVER;
	if (currentScene == SCENE::INTRO)
		return ANIMATION::BACKGROUND_INTRO;

  return 0;
}
