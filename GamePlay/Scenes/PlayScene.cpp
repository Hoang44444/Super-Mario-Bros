#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "PlayScene.h"
#include "Mario.h"
#include "MarioKeyHandler.h"
#include "Background.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "AnimationManager.h"
#include "debug.h"
#include "GameManager.h"
#include "Camera.h"
#include "Platform.h"
#include "Pipe.h"
#include "SwitchScenePoint.h"
#include "BrickTest.h"
#include "Brick.h"
#include "QuestionBlock.h"
#include "DynamicPlatform.h"
#include "Mushroom.h"
#include "FireFlower.h"
#include "PoisonMushroom.h"
#include "SuperStar.h"
#include "FrogSuit.h"
#include "CastleBridge.h"
#include "Axe.h"
// Items
#include "Coin.h"
#include "Mushroom1Up.h"
#include "SuperLeaf.h"
#include "HammerSuit.h"
#include "TanookiSuit.h"
#include "Hammer.h"
// Enemies
#include "Goomba.h"
#include "Koopa.h"
#include "BuzzyBeetle.h"
#include "HammerBro.h"
#include "Blooper.h"
#include "Bowser.h"
#include "BowserFire.h"
#include "BowserFireball.h"
#include "BulletBill.h"
#include "Cannon.h"
#include "Lakitu.h"
#include "Podoboo.h"
#include "Spiny.h"
#include "PiranhaPlant.h"
using namespace std;

void PlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath.c_str());

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

	int section = SCENE::SECTION_UNKNOWN;

	char str[1024];
	while (f.getline(str, 1024))
	{
		string line(str);

		// Strip UTF-8 BOM (EF BB BF) nếu có ở đầu dòng
		if (line.size() >= 3 &&
			(unsigned char)line[0] == 0xEF &&
			(unsigned char)line[1] == 0xBB &&
			(unsigned char)line[2] == 0xBF)
			line = line.substr(3);

		// Strip carriage return '\r' cuối dòng (Windows CRLF)
		if (!line.empty() && line.back() == '\r')
			line.pop_back();

		if (line.empty() || line[0] == '#') continue;

		if (line == "[ASSETS]") { section = SCENE::SECTION_ASSETS; continue; }
		if (line == "[OBJECTS]") { section = SCENE::SECTION_OBJECTS; continue; }
		if (line == "[MAP]") { section = SCENE::SECTION_MAP; continue; }
		if (line[0] == '[') { section = SCENE::SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case SCENE::SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE::SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE::SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();
	DebugOut(L"[INFO] Done loading scene from %s\n", sceneFilePath.c_str());
}

void PlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens;
	stringstream ss(line);
	string token;
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

	int section = ASSET::SECTION_UNKNOWN;

	char str[1024];
	while (f.getline(str, 1024))
	{
		string line(str);

		// Strip UTF-8 BOM
		if (line.size() >= 3 &&
			(unsigned char)line[0] == 0xEF &&
			(unsigned char)line[1] == 0xBB &&
			(unsigned char)line[2] == 0xBF)
			line = line.substr(3);

		// Strip carriage return '\r' cuối dòng
		if (!line.empty() && line.back() == '\r')
			line.pop_back();

		if (line.empty() || line[0] == '#') continue;

		if (line == "[SPRITES]") { section = ASSET::SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = ASSET::SECTION_ANIMATIONS; continue; }
		if (line == "[OBJECTS]") { section = SCENE::SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = ASSET::SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case ASSET::SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSET::SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE::SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();
	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void PlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens;
	stringstream ss(line);
	string token;
	while (ss >> token) tokens.push_back(token);

	if (tokens.size() < 6) return;

	int id = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = TextureManager::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	SpriteManager::GetInstance()->Add(id, l, t, r, b, tex);
}

void PlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens;
	stringstream ss(line);
	string token;
	
	// Cắt token cẩn thận, loại bỏ phần comment (bắt đầu bằng '#')
	while (ss >> token) 
	{
		if (token.empty() || token[0] == '#') break; // Bỏ qua comment inline
		tokens.push_back(token);
	}

	if (tokens.size() < 3) return; 

	int ani_id = atoi(tokens[0].c_str());
	LPANIMATION ani = new Animation();

	// Token[0] là ID animation
	// Token[i] là sprite_id, Token[i+1] là frame_time
	for (size_t i = 1; i + 1 < tokens.size(); i += 2)
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		
		LPSPRITE sprite = SpriteManager::GetInstance()->Get(sprite_id);
		
		if (sprite == nullptr)
		{
			DebugOut(L"[ERROR] Sprite ID %d not found in animation %d\n", sprite_id, ani_id);
			continue;
		}
		
		ani->Add(sprite, frame_time);
	}

	AnimationManager::GetInstance()->Add(ani_id, ani);
	DebugOut(L"[INFO] Animation ID %d added\n", ani_id);
}

void PlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens;
	stringstream ss(line);
	string token;
	while (ss >> token) tokens.push_back(token);

	if (tokens.size() < 4) return;

	int type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());
	float z = (float)atof(tokens[3].c_str());
	GameObject* obj = NULL;

		switch (type)
	{
	case OBJECT::MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before! \n");
			return;
		}
		obj = new Mario(x, y, z);
		player = obj;
		fixedCameraY = y;
		break;

	case OBJECT::PLATFORM:
		obj = new Platform(x, y, z);
		break;

	case OBJECT::BRICK_TEST:
		obj = new BrickTest(x, y, z);
		break;

	case OBJECT::BACKGROUND:
		obj = new Background(x, y, z);
		break;
	case OBJECT::PIPE:
	{
		int aniId = (tokens.size() >= 5) ? atoi(tokens[4].c_str()) : ANIMATION::PIPE_OVERWORLD;
		obj = new Pipe(x, y, z, aniId);
		break;
	}
	case OBJECT::SWITCH_SCENE_POINT:
		obj = new SwitchScenePoint(x, y, z);
		break;

	case OBJECT::BRICK:
	{
		int aniId = (tokens.size() >= 5) ? atoi(tokens[4].c_str()) : ANIMATION::BRICK_OVERWORLD;
		obj = new Brick(x, y, z, aniId);
		break;
	}

	case OBJECT::QUESTION_BLOCK:
		obj = new QuestionBlock(x, y, z);
		break;

	case OBJECT::MUSHROOM:
		obj = new Mushroom(x, y, z);
		break;

	case OBJECT::FIRE_FLOWER:
		obj = new FireFlower(x, y, z);
		break;

	case OBJECT::POISON_MUSHROOM:
		obj = new PoisonMushroom(x, y, z);
		break;

	case OBJECT::SUPER_STAR:
		obj = new SuperStar(x, y, z);
		break;

	case OBJECT::FROG_SUIT:
		obj = new FrogSuit(x, y, z);
		break;

	case OBJECT::CASTLE_BRIDGE:
		obj = new CastleBridge(x, y, z);
		break;

	case OBJECT::AXE:
		obj = new Axe(x, y, z);
		break;

	case OBJECT::DYNAMIC_PLATFORM:
	{
		if (tokens.size() < 5)
		{
			DebugOut(L"[ERROR] DYNAMIC_PLATFORM missing objectType field!\n");
			return;
		}
		int objectType = atoi(tokens[4].c_str());
		int initialDirection = (tokens.size() >= 6) ? atoi(tokens[5].c_str()) : 1;
		obj = new DynamicPlatform(x, y, z, objectType, initialDirection);
		break;
	}

	// ---- ITEMS ----
	case OBJECT::COIN:
		obj = new Coin(x, y, z);
		break;

	case OBJECT::MUSHROOM_1UP:
		obj = new Mushroom1Up(x, y, z);
		break;

	case OBJECT::SUPER_LEAF:
		obj = new SuperLeaf(x, y, z);
		break;

	case OBJECT::HAMMER_SUIT:
		obj = new HammerSuit(x, y, z);
		break;

	case OBJECT::TANOOKI_SUIT:
		obj = new TanookiSuit(x, y, z);
		break;

	case OBJECT::HAMMER:
		obj = new Hammer(x, y, z);
		break;

	// ---- ENEMIES ----
	case OBJECT::GOOMBA:
		obj = new Goomba(x, y, z);
		break;

	case OBJECT::KOOPA:
		obj = new Koopa(x, y, z);
		break;

	case OBJECT::BUZZY_BEETLE:
		obj = new BuzzyBeetle(x, y, z);
		break;

	case OBJECT::HAMMER_BRO:
		obj = new HammerBro(x, y, z);
		break;

	case OBJECT::BLOOPER:
		obj = new Blooper(x, y, z);
		break;

	case OBJECT::BOWSER:
		obj = new Bowser(x, y, z);
		break;

	case OBJECT::BOWSER_FIRE:
		obj = new BowserFire(x, y, z);
		break;

	case OBJECT::BOWSER_FIREBALL:
		obj = new BowserFireball(x, y, z);
		break;

	case OBJECT::BULLET_BILL:
		obj = new BulletBill(x, y, z);
		break;

	case OBJECT::CANNON:
		obj = new Cannon(x, y, z);
		break;

	case OBJECT::LAKITU:
		obj = new Lakitu(x, y, z);
		break;

	case OBJECT::PODOBOO:
		obj = new Podoboo(x, y, z);
		break;

	case OBJECT::SPINY:
		obj = new Spiny(x, y, z);
		break;

	case OBJECT::PIRANHA_PLANT:
		obj = new PiranhaPlant(x, y, z);
		break;
	}

	if (obj != NULL) {
		obj->SetScene(this);
		objects.push_back(obj);
	}
}

void PlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens;
	stringstream ss(line);
	string token;
	while (ss >> token) tokens.push_back(token);

	if (tokens.size() < 2) return;

	int width = atoi(tokens[0].c_str());
	int height = atoi(tokens[1].c_str());

	Camera::GetInstance()->SetMapSize(width, height);
}

void PlayScene::StartCastleEndSequence()
{
	if (castleEndSequenceStarted) return;
	castleEndSequenceStarted = true;
	castleEndTimer = 2600;

	DWORD bridgeDelay = 0;
	for (size_t i = 0; i < objects.size(); i++)
	{
		CastleBridge* bridge = dynamic_cast<CastleBridge*>(objects[i]);
		if (bridge != nullptr) { bridge->Collapse(bridgeDelay); bridgeDelay += 120; }
	}

	if (player != nullptr)
		player->SetState(MARIO_STATE::WALKING_RIGHT);
}

void PlayScene::Update(DWORD dt)
{
	if (castleEndSequenceStarted)
	{
		if (castleEndTimer > dt) castleEndTimer -= dt;
		else GameManager::GetInstance()->InitiateSwitchScene(SCENE::END);
	}

	vector<LPGAMEOBJECT> coObjects;
	for (auto obj : objects) coObjects.push_back(obj);

	// Update every object EXCEPT the player first, then update the player last.
	// The player must run after the moving platforms so its collision resolves
	// against the platforms' current-frame position. Otherwise Mario lags one
	// frame behind and falls through / jitters on upward-moving platforms.
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i] == player) continue;
		if (!objects[i]->IsDeleted())
			objects[i]->Update(dt, &coObjects);
	}

	if (player != nullptr && !player->IsDeleted())
		player->Update(dt, &coObjects);

	// skip the rest if scene was already unloaded (Mario died)
	if (player == nullptr) return;

	// Update camera to follow mario
	float cx, cy, cz;
	player->GetPosition(cx, cy, cz);

	Camera::GetInstance()->Follow(cx, fixedCameraY);

	// Remove deleted objects
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->IsDeleted())
		{
			delete objects[i];
			objects.erase(objects.begin() + i);
			i--;
		}
	}
}

void PlayScene::Render()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}
}

void PlayScene::Unload()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();
	player = NULL;
}
