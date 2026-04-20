#include <iostream>
#include <fstream>
#include <string>

#include "PlayScene.h"
#include "BrickTest.h"
#include "TextureManager.h"
#include "debug.h"

using namespace std;

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_OBJECTS 3

#define MAX_SCENE_LINE 1024

void PlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);
		
	ifstream f;
	f.open(sceneFilePath);

	if (!f.is_open())
	{
		DebugOut(L"[ERROR] Failed to open scene file: %s\n", sceneFilePath);
		return;
	}

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		// remove carriage return at the end of line (Windows fix)
		if (!line.empty() && line.back() == '\r') {
			line.pop_back();
		}

		if (line.empty() || line[0] == '#') continue;	// skip empty or comment lines	

		if (line == "[TEXTURES]") { 
			section = SCENE_SECTION_TEXTURES; 
			DebugOut(L"[INFO] Entering section [TEXTURES]\n");
			continue; 
		}
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; 
			DebugOut(L"[INFO] Entering section [OBJECTS]\n");
			continue; 
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: {
			char path[MAX_SCENE_LINE];
			int id;
			if (sscanf_s(str, "%d %s", &id, path, (unsigned int)sizeof(path)) == 2) {
				wstring wpath = wstring(path, path + strlen(path));
				DebugOut(L"[INFO] Loading texture ID %d from: %s\n", id, wpath.c_str());
				TextureManager::GetInstance()->Add(id, wpath.c_str());
			}
			break;
		}
		case SCENE_SECTION_OBJECTS: {
			int type;
			float x, y, left, right, vx;
			if (sscanf_s(str, "%d %f %f %f %f %f", &type, &x, &y, &left, &right, &vx) == 6) {
				GameObject* obj = NULL;

				switch (type)
				{
				case 1: // BRICK
					obj = new Brick(x, y, left, right, vx);
					LPTEXTURE tex = TextureManager::GetInstance()->Get(0);
					if (tex != NULL) {
						((Brick*)obj)->SetTexture(tex);
						DebugOut(L"[INFO] Brick created at (%.1f, %.1f) with texture ID 0\n", x, y);
					} else {
						DebugOut(L"[ERROR] Failed to get texture ID 0 for Brick!\n");
					}
					break;
				}

				if (obj != NULL)
					objects.push_back(obj);
			}
			break;
		}
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene from %s\n", sceneFilePath);
}

void PlayScene::Update(DWORD dt)
{
	DebugOut(L"[INFO] Start updating scene from : %s \n", sceneFilePath);
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt);
	}
	DebugOut(L"[INFO] Done updating scene from : %s \n", sceneFilePath);
}

void PlayScene::Render()
{
	DebugOut(L"[INFO] Start rendering scene from : %s \n", sceneFilePath);
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}
	DebugOut(L"[INFO] Done rendering scene from : %s \n", sceneFilePath);
}

void PlayScene::Unload()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();
}
