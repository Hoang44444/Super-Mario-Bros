#include "SceneManager.h"
#include <string>
SceneManager* SceneManager::__instance = NULL;

SceneManager* SceneManager::GetInstance()
{
	if (__instance == NULL) __instance = new SceneManager();
	return __instance;
}

void SceneManager::Add(int id, LPSCENE scene)
{
	scenes[id] = scene;
}

void SceneManager::SwitchScene(int id)
{
	if (scenes.find(current_scene) != scenes.end())
		scenes[current_scene]->Unload();

	current_scene = id;
	scenes[current_scene]->Load();
}

void SceneManager::Update(DWORD dt)
{
	if (scenes.find(current_scene) != scenes.end())
		scenes[current_scene]->Update(dt);


}

void SceneManager::Render()
{
	if (scenes.find(current_scene) != scenes.end())
		scenes[current_scene]->Render();
}
