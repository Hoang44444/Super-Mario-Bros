#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "PlayScene.h"
#include "Mario.h"
#include "Goomba.h"
#include "MarioKeyHandler.h"
#include "BrickTest.h"
#include "Troopa.h"
#include "../Resource/TextureManager.h"
#include "../Resource/SpriteManager.h"
#include "../Graphic/AnimationManager.h"
#include "Portal.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "../Resource/debug.h"
#include "GameManager.h"
#include "../Graphic/Camera.h"
#include "Mushroom.h"
#include "Coin.h"
#include "Star.h"
#include "FireFlower.h"
#include "OneUpMushroom.h"


using namespace std;

void PlayScene::Load()
{
	Mushroom* mushroom = new Mushroom(100.0f, 50.0f);

	if (key_handler == NULL)
	{
		key_handler = new MarioKeyHandler(this);
		GameManager::GetInstance()->SetKeyHandler(key_handler);
	}

	ifstream f;
	f.open(sceneFilePath.c_str());
	if (!f.is_open())
	{
		DebugOut(L"[ERROR] Failed to open scene file: %s\n", sceneFilePath.c_str());
		return;
	}

	int section = SCENE_SECTION_UNKNOWN;
	char str[1024];
	while (f.getline(str, 1024))
	{
		string line(str);
		// Strip Windows \r
		if (!line.empty() && line.back() == '\r') line.pop_back();
		if (line.empty() || line[0] == '#') continue;

		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS;  continue; }
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case SCENE_SECTION_ASSETS:  _ParseSection_ASSETS(line);  break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}
	f.close();
	DebugOut(L"[INFO] Done loading scene from %s\n", sceneFilePath.c_str());

	// Camera setup - called every scene load.
	// Always use internal resolution (282x240), not window size (1920x1080).
	Camera::GetInstance()->SetSize(282, 240);

	// Set map width based on scene id.
	// Add a new line here whenever you add a new scene.
	int mapWidth = 3408;          // scene 1 = World 1-1  (212 tiles)
	if (id == 2) mapWidth = 2752; // scene 2 = World 1-2  (172 tiles)
	if (id == 3) mapWidth = 2496; // scene 3 = World 1-3  (156 tiles)
	if (id == 4) mapWidth = 800;  // scene 4 = stub
	Camera::GetInstance()->SetMapSize(mapWidth, 240);

	// Always reset camera to x=0 when a new scene starts.
	Camera::GetInstance()->SetPosition(0, 0);
}

void PlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens;
	stringstream ss(line); string token;
	while (ss >> token) tokens.push_back(token);
	if (tokens.size() < 1) return;

	wstring path = wstring(tokens[0].begin(), tokens[0].end());
	LoadAssets(path.c_str());
}

void PlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);
	if (!f.is_open())
	{
		DebugOut(L"[ERROR] Failed to open asset file: %s\n", assetFile);
		return;
	}

	int section = ASSET_SECTION_UNKNOWN;
	char str[1024];
	while (f.getline(str, 1024))
	{
		string line(str);
		if (!line.empty() && line.back() == '\r') line.pop_back();
		if (line.empty() || line[0] == '#') continue;

		if (line == "[SPRITES]") { section = ASSET_SECTION_SPRITES;    continue; }
		if (line == "[ANIMATIONS]") { section = ASSET_SECTION_ANIMATIONS; continue; }
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS;    continue; }
		if (line[0] == '[') { section = ASSET_SECTION_UNKNOWN;    continue; }

		switch (section)
		{
		case ASSET_SECTION_SPRITES:    _ParseSection_SPRITES(line);    break;
		case ASSET_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_OBJECTS:    _ParseSection_OBJECTS(line);    break;
		}
	}
	f.close();
	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void PlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens;
	stringstream ss(line); string token;
	while (ss >> token) tokens.push_back(token);
	if (tokens.size() < 6) return;

	int id = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = TextureManager::GetInstance()->Get(texID);
	if (tex == NULL) { DebugOut(L"[ERROR] Texture ID %d not found!\n", texID); return; }
	SpriteManager::GetInstance()->Add(id, l, t, r, b, tex);
}

void PlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens;
	stringstream ss(line); string token;
	while (ss >> token) tokens.push_back(token);
	if (tokens.size() < 3) return;

	int ani_id = atoi(tokens[0].c_str());
	LPANIMATION ani = new Animation();
	for (int i = 1; i < (int)tokens.size(); i += 2)
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(SpriteManager::GetInstance()->Get(sprite_id), frame_time);
	}
	AnimationManager::GetInstance()->Add(ani_id, ani);
}

void PlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens;
	stringstream ss(line); string token;
	while (ss >> token) tokens.push_back(token);
	if (tokens.size() < 3) return;

	int   type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	GameObject* obj = NULL;

	switch (type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL) { DebugOut(L"[ERROR] MARIO already created!\n"); return; }
		obj = new Mario(x, y);
		player = obj;
		break;

	case OBJECT_TYPE_BRICK:
	{
		// Optional 4th param: animId (default = ID_ANI_BRICK = 100)
		// Format: 1  x  y  [animId]
		int animId = (tokens.size() >= 4) ? atoi(tokens[3].c_str()) : ID_ANI_BRICK;
		obj = new Brick(x, y, animId);
		break;
	case OBJECT_TYPE_MUSHROOM:
		obj = new Mushroom(x, y);
		break;
	case OBJECT_TYPE_COIN:
		obj = new Coin(x, y);
		break;
	case OBJECT_TYPE_FIRE_FLOWER:
		obj = new FireFlower(x, y);
		break;
	case OBJECT_TYPE_STAR:
		obj = new Star(x, y);
		break;
	case OBJECT_TYPE_ONE_UP_MUSHROOM:
		obj = new OneUpMushroom(x, y);
		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		// Format: 50  x  y  width  height  scene_id
		if (tokens.size() < 6) break;
		float w = (float)atof(tokens[3].c_str());
		float h = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new Portal(x, y, w, h, scene_id);
		break;
	case OBJECT_TYPE_GOOMBA:
		obj = new Goomba(x, y);
		break;
	case OBJECT_TYPE_TROOPA:
		obj = new Troopa(x, y);
		break;
	}

	default:
		DebugOut(L"[WARNING] Unknown object type: %d\n", type);
		break;
	}

	if (obj != NULL)
	{
		obj->SetScene(this);
		objects.push_back(obj);
	}
}

void PlayScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (auto obj : objects) coObjects.push_back(obj);

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->IsDeleted())
			objects[i]->Update(dt, &coObjects);
	}

	// Portal overlap check
	if (player != NULL)
	{
		float ml, mt, mr, mb;
		player->GetBoundingBox(ml, mt, mr, mb);

		for (size_t i = 0; i < objects.size(); i++)
		{
			Portal* p = dynamic_cast<Portal*>(objects[i]);
			if (!p) continue;

			float pl, pt, pr, pb;
			p->GetBoundingBox(pl, pt, pr, pb);

			if (mr > pl && ml < pr && mb > pt && mt < pb)
			{
				GameManager::GetInstance()->InitiateSwitchScene(p->GetSceneId());
				break;
			}
		}
	}

	// Clean up deleted objects
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->IsDeleted())
		{
			delete objects[i];
			objects.erase(objects.begin() + i);
			i--;
		}
	}

	// Camera follow: keep Mario ~80px from the left edge, never scroll back.
	if (player != NULL)
	{
		float px, py;
		player->GetPosition(px, py);

		float camX = px - 80.0f;

		// Use Camera's own map width so this works for every scene
		float maxCamX = (float)(Camera::GetInstance()->GetMapWidth() - 282);
		if (camX < 0) camX = 0;
		if (maxCamX > 0 && camX > maxCamX) camX = maxCamX;

		// Only move right, never scroll back left
		if (camX > Camera::GetInstance()->GetX())
			Camera::GetInstance()->SetPosition(camX, 0);
	}

	GameManager::GetInstance()->SwitchScene();
}

void PlayScene::Render()
{
	for (size_t i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

void PlayScene::Unload()
{
	for (size_t i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();
	player = NULL;
}