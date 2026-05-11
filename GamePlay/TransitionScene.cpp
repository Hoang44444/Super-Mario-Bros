#include "TransitionScene.h"
#include "GameManager.h"
#include "Camera.h"
#include "debug.h"

void TransitionScene::Load()
{
	DebugOut(L"[INFO] TransitionScene %d loaded (target=%d)\n", id, target_scene_id);

	elapsed = 0;
	started = true;

	// Lock camera at origin - transition screen fills the viewport
	Camera::GetInstance()->SetSize(282, 240);
	Camera::GetInstance()->SetMapSize(282, 240);
	Camera::GetInstance()->SetPosition(0, 0);
}

void TransitionScene::Unload()
{
	started = false;
	DebugOut(L"[INFO] TransitionScene %d unloaded\n", id);
}

void TransitionScene::Update(DWORD dt)
{
	if (!started) return;

	elapsed += dt;
	if (elapsed >= display_ms)
	{
		// Time's up - switch to the target level
		GameManager::GetInstance()->InitiateSwitchScene(target_scene_id);
		GameManager::GetInstance()->SwitchScene();
	}
}

void TransitionScene::Render()
{
	// Draw the transition image full-screen at world position (0, 0).
	// The camera is locked to (0,0) and the image is 282x240 = exact internal resolution.
	LPTEXTURE tex = TextureManager::GetInstance()->Get(tex_id);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] TransitionScene: texture id=%d not found\n", tex_id);
		return;
	}

	Renderer::GetInstance()->Draw(0, 0, tex);
}