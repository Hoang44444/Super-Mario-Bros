#pragma once
#include "Scene.h"
#include "TextureManager.h"
#include "Renderer.h"

// Displays a black "WORLD X-X" screen for a fixed duration,
// then automatically switches to target_scene_id.
//
// Usage: register in super-mario-bros.txt as a scene,
// then point portals at the transition scene instead of the level directly.
// The transition scene itself knows which level to go to next.

class TransitionScene : public Scene
{
	int         target_scene_id;   // scene to switch to after timer expires
	DWORD       display_ms;        // how long to show the screen (milliseconds)
	DWORD       elapsed;           // accumulated time
	int         tex_id;            // texture ID of the transition image
	bool        started;

public:
	TransitionScene(int id, LPCWSTR filePath, int target_scene_id, int tex_id, DWORD display_ms = 3000)
		: Scene(id, filePath)
	{
		this->target_scene_id = target_scene_id;
		this->tex_id = tex_id;
		this->display_ms = display_ms;
		this->elapsed = 0;
		this->started = false;
	}

	virtual void Load()   override;
	virtual void Unload() override;
	virtual void Update(DWORD dt) override;
	virtual void Render() override;
	virtual void AddObject(LPGAMEOBJECT obj) override {} // unused
};