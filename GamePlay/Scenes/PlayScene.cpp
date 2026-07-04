#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "PlayScene.h"
#include "Mario.h"
#include "HUD.h"
#include "MenuHUD.h"
#include "MarioKeyHandler.h"
#include "MenuKeyHandler.h"
#include "Background.h"
#include "MenuOptions.h"
#include "LivesNumber.h"
#include "PlayerData.h"
#include "../Resource/AssetID.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "AnimationManager.h"
#include "debug.h"
#include "GameManager.h"
#include "Camera.h"
#include "Platform.h"
#include "Pipe.h"
#include "SwitchScenePoint.h"
#include "EnemyTurnBlock.h"
#include "BrickTest.h"
#include "Brick.h"
#include "QuestionBlock.h"
#include "DynamicPlatform.h"
#include "Mushroom.h"
#include "FireFlower.h"
#include "PoisonMushroom.h"
#include "SuperStar.h"
#include "CastleBridge.h"
#include "Axe.h"
// Items
#include "../Coin.h"
#include "../ItemCoin2.h"
#include "../Mushroom1Up.h"
// Enemies
#include "../Goomba.h"
#include "../Koopa.h"
#include "../BuzzyBeetle.h"
#include "../HammerBro.h"
#include "../Blooper.h"
#include "../Bowser.h"
#include "../BulletBill.h"
#include "../Cannon.h"
#include "../Lakitu.h"
#include "../Podoboo.h"
#include "../Spiny.h"
#include "../PiranhaPlant.h"
#include "../../Resource/SoundManager.h"
// Wind Effect
#include "../WindEffect.h"
#include "../WindCycle.h"
using namespace std;

namespace
{
	constexpr ULONGLONG MARIO_DIE_SCENE_DELAY_MS = 3000;
	constexpr ULONGLONG GAME_OVER_SCENE_DELAY_MS = 4000;
}

PlayScene::PlayScene(int id, LPCWSTR filePath) : Scene(id, filePath)
{
}

// Load scene: parse file scene, tạo objects, setup key handler, HUD, BGM, và effects
void PlayScene::Load()
{
	marioDieStart = 0; // fresh load -> Mario is alive again
	sceneStart = GetTickCount64();
	windSfxPlaying = false;  // reset wind SFX state
	earthquakeSfxPlaying = false;  // reset earthquake SFX state

	if (key_handler == NULL)
	{
	// Non-gameplay screens (menu / control / end / death) không có player; dùng menu handler.
		if (id == SCENE::MENU || id == SCENE::CONTROL || id == SCENE::END || id == SCENE::DEATH || id == SCENE::GAME_OVER || id == SCENE::INTRO)
			key_handler = new MenuKeyHandler();
		else
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
	// Determine world và stage cho HUD dựa trên scene hiện tại hoặc returnScene
	int hudWorld = 1;
	int hudStage = 1;
	if (id >= SCENE::WORLD_1_1 && id <= SCENE::WORLD_1_4)
	{
		hudWorld = 1;
		hudStage = id;
	}
	else if (id == SCENE::INTRO || id == SCENE::DEATH)
	{
		int levelId = PlayerData::Get().returnScene;
		hudWorld = 1;
		hudStage = levelId;
	}

	delete hud;
	if (id >= SCENE::WORLD_1_1 && id <= SCENE::WORLD_1_4 && player != nullptr)
	{
		hud = new HUD(static_cast<Mario*>(player), hudWorld, hudStage);
	}
	else
	{
		hud = new HUD(nullptr, hudWorld, hudStage);
	}

	if (id == SCENE::MENU)
	{
		delete menuHUD;
		menuHUD = new MenuHUD();
	}

	// Load SFX
	SoundManager::GetInstance()->LoadSFX(SFX::JUMP, "../Resource/audio/sfx/smb_jump-super.wav");
	SoundManager::GetInstance()->LoadSFX(SFX::STOMP, "../Resource/audio/sfx/smb_stomp.wav");
	SoundManager::GetInstance()->LoadSFX(SFX::COIN, "../Resource/audio/sfx/smb_coin.wav");
	SoundManager::GetInstance()->LoadSFX(SFX::DIE, "../Resource/audio/bgm/smb_mariodie.wav");
	SoundManager::GetInstance()->LoadSFX(SFX::POWERUP, "../Resource/audio/sfx/smb_powerup.wav");
	SoundManager::GetInstance()->LoadSFX(SFX::POWERUP_APPEARS, "../Resource/audio/sfx/smb_powerup_appears.wav");
	SoundManager::GetInstance()->LoadSFX(SFX::FIREBALL, "../Resource/audio/sfx/smb_fireball.wav");
	SoundManager::GetInstance()->LoadSFX(SFX::BRICK_BREAK, "../Resource/audio/sfx/smb_breakblock.wav");
	SoundManager::GetInstance()->LoadSFX(SFX::ONE_UP, "../Resource/audio/sfx/smb_1-up.wav");
	SoundManager::GetInstance()->LoadSFX(SFX::SMB_WINDY, "../Resource/audio/sfx/smb_windy.wav");
	SoundManager::GetInstance()->LoadSFX(SFX::SMB_SHAKE, "../Resource/audio/sfx/smb_shake.wav");
	SoundManager::GetInstance()->LoadSFX(SFX::FIREWORKS, "../Resource/audio/sfx/smb_fireworks.wav");

	// Load và play BGM dựa trên loại scene
	if (id >= SCENE::WORLD_1_1 && id <= SCENE::WORLD_1_4)
	{
		// Load tất cả BGM có thể cho world 1 levels
		SoundManager::GetInstance()->LoadBGM(BGM::OVERWORLD_THEME, "../Resource/audio/bgm/overworld_theme.wav");
		SoundManager::GetInstance()->LoadBGM(BGM::UNDERWORLD_THEME, "../Resource/audio/bgm/underworld_theme.wav");
		SoundManager::GetInstance()->LoadBGM(BGM::CASTLE_THEME, "../Resource/audio/bgm/castle_theme.wav");
		SoundManager::GetInstance()->LoadBGM(BGM::STAR_THEME, "../Resource/audio/bgm/star_theme.wav");
		SoundManager::GetInstance()->LoadBGM(BGM::WARNING_THEME, "../Resource/audio/bgm/smb_warning.wav");
		SoundManager::GetInstance()->LoadBGM(BGM::STAGE_CLEAR_THEME, "../Resource/audio/bgm/smb_stage_clear.wav");

		// Play BGM phù hợp dựa trên level
		if (id == SCENE::WORLD_1_2)
		{
			SoundManager::GetInstance()->PlayBGM(BGM::UNDERWORLD_THEME, true);
		}
		else if (id == SCENE::WORLD_1_4)
		{
			SoundManager::GetInstance()->PlayBGM(BGM::CASTLE_THEME, true);
		}
		else
		{
			// WORLD_1_1 và WORLD_1_3 dùng overworld theme
			SoundManager::GetInstance()->PlayBGM(BGM::OVERWORLD_THEME, true);
		}
	}
	else if (id == SCENE::GAME_OVER)
	{
		SoundManager::GetInstance()->LoadBGM(BGM::GAME_OVER_THEME, "../Resource/audio/bgm/smb_gameover.wav");
		SoundManager::GetInstance()->PlayBGM(BGM::GAME_OVER_THEME, false);
	}

	SoundManager::GetInstance()->ApplyVolumeSettings();

	// Hiệu ứng động đất ở màn cuối (Bowser): camera rung từng đợt cho tới khi
	// Mario đứng được lên cầu. Các màn khác đảm bảo tắt rung (camera dùng chung).
	if (id == SCENE::WORLD_1_4)
		Camera::GetInstance()->StartEarthquake();
	else
		Camera::GetInstance()->StopEarthquake();

	// Gió ở màn 1-3: bật chu kỳ gió dùng chung (đồng bộ lá + lực đẩy Mario).
	// Các màn khác tắt để không sót trạng thái.
	if (id == SCENE::WORLD_1_3)
	{
		WindCycle::GetInstance()->Start();
	}
	else
	{
		WindCycle::GetInstance()->Stop();
	}

	// Boss màn 1-4: tìm Bowser để kích hoạt khi Mario bước lên cầu (mặc định nó đứng yên).
	bossBowser = nullptr;
	if (id == SCENE::WORLD_1_4)
	{
		for (auto obj : objects)
		{
			if (dynamic_cast<Bowser*>(obj) != nullptr) { bossBowser = obj; break; }
		}
	}
}

// Parse section [ASSETS] từ file scene - load file asset
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

// Load assets từ file: parse sprites, animations, và objects
void PlayScene::LoadAssets(LPCWSTR assetFile)
{
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
}

// Parse section [SPRITES] - tạo sprite từ texture
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

// Parse section [ANIMATIONS] - tạo animation từ sprites
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
}

// Parse section [OBJECTS] - tạo objects dựa trên type
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
		if (this->id == SCENE::WORLD_1_3)
		{
			static_cast<Mario*>(player)->SetWindyScene(true);
		}
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

	case OBJECT::ENEMY_TURN_BLOCK:
		obj = new EnemyTurnBlock(x, y, z);
		break;

	case OBJECT::BRICK:
	{
		int aniId = (tokens.size() >= 5) ? atoi(tokens[4].c_str()) : ANIMATION::BRICK_OVERWORLD;
		obj = new Brick(x, y, z, aniId);
		break;
	}

	case OBJECT::QUESTION_BLOCK:
	{
		int itemType = (tokens.size() >= 5) ? atoi(tokens[4].c_str()) : OBJECT::COIN;
		obj = new QuestionBlock(x, y, z, itemType);
		break;
	}

	case OBJECT::MUSHROOM:
	case OBJECT::FIRE_FLOWER:
	case OBJECT::POISON_MUSHROOM:
	case OBJECT::SUPER_STAR:
		obj = CreateItem(type, x, y, z);
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
	case OBJECT::ITEM_COIN2:
	case OBJECT::MUSHROOM_1UP:
		obj = CreateItem(type, x, y, z);
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

	case OBJECT::BULLET_BILL:
	{
		int dir = (tokens.size() >= 5) ? atoi(tokens[4].c_str()) : -1;
		obj = new BulletBill(x, y, z);
		if (obj) static_cast<BulletBill*>(obj)->SetMovement(dir);
		break;
	}

	case OBJECT::CANNON:
	{
		int dir = (tokens.size() >= 5) ? atoi(tokens[4].c_str()) : 1;
		obj = new Cannon(x, y, z, dir);
		break;
	}

	case OBJECT::LAKITU:
	{
		float endX = (tokens.size() >= 5) ? (float)atof(tokens[4].c_str()) : 1e9f;
		obj = new Lakitu(x, y, z, endX);
		break;
	}

	case OBJECT::PODOBOO:
		obj = new Podoboo(x, y, z);
		break;

	case OBJECT::SPINY:
		obj = new Spiny(x, y, z);
		break;

	case OBJECT::PIRANHA_PLANT:
		obj = new PiranhaPlant(x, y, z);
		break;
	case OBJECT::WIND_PARTICLE:
		obj = new WindEffect(x, y, z);
		break;
	case OBJECT::MENU_OPTIONS:
	{
		// 5th token = path to the option-definition file (defaults if omitted).
		wstring optFile = L"Objects/menu_options.txt";
		if (tokens.size() >= 5)
			optFile = wstring(tokens[4].begin(), tokens[4].end());
		menuOptions = new MenuOptions(x, y, z, optFile.c_str());
		obj = menuOptions;
		break;
	}

	case OBJECT::LIVES_NUMBER:
	{
		// tokens: 51 x y z [w] [h]  — x,y là toạ độ logic; w,h là kích thước vẽ.
		float w = (tokens.size() >= 5) ? (float)atof(tokens[4].c_str()) : 11.0f;
		float h = (tokens.size() >= 6) ? (float)atof(tokens[5].c_str()) : 18.0f;
		obj = new LivesNumber(x, y, z, w, h);
		break;
	}
	}

	if (obj != NULL) {
		obj->SetScene(this);
		objects.push_back(obj);
	}
}

// Tạo item object dựa trên type
LPGAMEOBJECT PlayScene::CreateItem(int type, float x, float y, float z)
{
	switch (type)
	{
	case OBJECT::COIN:            return new Coin(x, y, z);
	case OBJECT::ITEM_COIN2:      return new ItemCoin2(x, y, z);
	case OBJECT::MUSHROOM:        return new Mushroom(x, y, z);
	case OBJECT::FIRE_FLOWER:     return new FireFlower(x, y, z);
	case OBJECT::POISON_MUSHROOM: return new PoisonMushroom(x, y, z);
	case OBJECT::SUPER_STAR:      return new SuperStar(x, y, z);
	case OBJECT::MUSHROOM_1UP:    return new Mushroom1Up(x, y, z);
	}
	return nullptr;
}

// Parse section [MAP] - set kích thước map cho camera
void PlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens;
	stringstream ss(line);
	string token;
	while (ss >> token) tokens.push_back(token);

	if (tokens.size() < 2) return;

	int width = atoi(tokens[0].c_str());
	int height = atoi(tokens[1].c_str());

	this->mapHeight = height;
	Camera::GetInstance()->SetMapSize(width, height);
}

// Update scene: xử lý logic game, collision, camera, effects
void PlayScene::Update(DWORD dt)
{
	if (id == SCENE::INTRO || id == SCENE::DEATH)
	{
		ULONGLONG now = GetTickCount64();
		if (now - sceneStart >= 2000)
		{
			GameManager::GetInstance()->InitiateSwitchScene(PlayerData::Get().returnScene);
			return;
		}
	}

	if (id == SCENE::GAME_OVER)
	{
		ULONGLONG now = GetTickCount64();
		if (now - sceneStart >= GAME_OVER_SCENE_DELAY_MS)
		{
			PlayerData::Get().Reset();
			GameManager::GetInstance()->InitiateSwitchScene(SCENE::MENU);
			return;
		}
	}

	// Boss màn 1-4: Bowser chỉ bắt đầu hoạt động khi Mario đã bước lên cầu.
	if (bossBowser != nullptr && !bossBowser->IsDeleted() && IsPlayerOnCastleBridge())
		static_cast<Bowser*>(bossBowser)->Activate();

	vector<LPGAMEOBJECT> coObjects;
	for (auto obj : objects) coObjects.push_back(obj);

	// Update mọi object TRỪ player trước, rồi update player cuối cùng.
	// Player phải chạy sau moving platforms để collision resolve với vị trí hiện tại của platforms.
	// Nếu không Mario sẽ lag 1 frame và rơi qua/jitter trên platforms di chuyển lên.
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i] == player) continue;
		if (!objects[i]->IsDeleted())
			objects[i]->Update(dt, &coObjects);
	}

	if (player != nullptr && !player->IsDeleted())
		player->Update(dt, &coObjects);

	if (hud != nullptr)
		hud->Update(dt);

	if (menuHUD != nullptr)
		menuHUD->Update();

	// Wind SFX: chỉ play khi gió thực sự thổi (không trong khoảng nghỉ)
	// Track state để tránh gọi PlaySFX mỗi frame (sẽ restart sound)
	bool windActive = (id == SCENE::WORLD_1_3 && WindCycle::GetInstance()->IsActive());
	if (windActive && !windSfxPlaying)
	{
		SoundManager::GetInstance()->PlaySFX(SFX::SMB_WINDY, true);
		windSfxPlaying = true;
	}
	else if (!windActive && windSfxPlaying)
	{
		SoundManager::GetInstance()->StopSFX(SFX::SMB_WINDY);
		windSfxPlaying = false;
	}

	// Earthquake SFX: chỉ play khi camera thực sự rung (không trong khoảng nghỉ hoặc khi trên cầu)
	bool earthquakeShaking = Camera::GetInstance()->IsEarthquakeShaking() && !IsPlayerOnCastleBridge();
	if (earthquakeShaking && !earthquakeSfxPlaying)
	{
		SoundManager::GetInstance()->PlaySFX(SFX::SMB_SHAKE, true);
		earthquakeSfxPlaying = true;
	}
	else if (!earthquakeShaking && earthquakeSfxPlaying)
	{
		SoundManager::GetInstance()->StopSFX(SFX::SMB_SHAKE);
		earthquakeSfxPlaying = false;
	}

	// Stage clear sequence: countdown thời gian với điểm bonus
	if (stageClearActive)
	{
		ULONGLONG now = GetTickCount64();
		constexpr ULONGLONG STAGE_CLEAR_BGM_DURATION = 2000;  // 2 giây cho stage clear BGM
		constexpr ULONGLONG COUNTDOWN_SPEED = 30;  // countdown mỗi 30ms (nhanh hơn)
		constexpr int TIME_BONUS_MULTIPLIER = 50;  // 50 điểm mỗi giây

		ULONGLONG elapsed = now - stageClearStart;

		// Phase 1: Play stage clear BGM trong 2 giây
		if (elapsed < STAGE_CLEAR_BGM_DURATION)
		{
			// Just wait, BGM is playing
		}
		// Phase 2: Countdown thời gian nhanh với điểm + loop COIN SFX
		else if (stageClearDisplayTime > 0)
		{
			ULONGLONG countdownElapsed = elapsed - STAGE_CLEAR_BGM_DURATION;
			int countdownSteps = (int)(countdownElapsed / COUNTDOWN_SPEED);
			int timeToSubtract = countdownSteps * 3;  // Giảm 3 giây mỗi lần countdown

			if (timeToSubtract > stageClearDisplayTime)
				timeToSubtract = stageClearDisplayTime;

			if (timeToSubtract > 0)
			{
				int oldDisplayTime = stageClearDisplayTime;
				stageClearDisplayTime -= timeToSubtract;
				if (stageClearDisplayTime < 0) stageClearDisplayTime = 0;

				// Tính điểm: thời gian còn lại thực tế x 50
				// Mỗi lần countdown giảm 3 giây hiển thị, nhưng điểm dựa trên stageClearTime thực tế
				int scoreToAdd = stageClearTime * TIME_BONUS_MULTIPLIER;
				ScoreManager::Get().AddScore(scoreToAdd);

				// Play COIN SFX
				SoundManager::GetInstance()->PlaySFX(SFX::COIN, false);

				// Update HUD time display
				if (hud != nullptr)
				{
					hud->SetRemainingTime(stageClearDisplayTime);
				}

				// Reset stageClearStart để track bước countdown tiếp theo
				stageClearStart = now - STAGE_CLEAR_BGM_DURATION;
			}
		}
		// Phase 3: Thời gian về 0, play FIREWORKS sound rồi chuyển
		else
		{
			constexpr ULONGLONG FIREWORKS_DURATION = 3000;  // 3 giây cho fireworks
			constexpr ULONGLONG FIREWORKS_START = STAGE_CLEAR_BGM_DURATION;

			if (elapsed < FIREWORKS_START + FIREWORKS_DURATION)
			{
				// Play fireworks sound một lần
				static bool fireworksPlayed = false;
				if (!fireworksPlayed)
				{
					SoundManager::GetInstance()->PlaySFX(SFX::FIREWORKS, false);
					fireworksPlayed = true;
				}
			}
			else
			{
				// Fireworks xong, chuyển sang INTRO scene
				stageClearActive = false;
				if (hud != nullptr)
					hud->SetStageClearActive(false);  // Re-enable warning BGM logic

				// Make Mario visible again for the next level
				if (player != nullptr)
					player->SetVisible(true);

				GameManager::GetInstance()->InitiateSwitchScene(SCENE::INTRO);
				return;
			}
		}
	}

	// Bỏ qua phần còn lại nếu scene đã unloaded (Mario chết)
	if (player == nullptr) return;

	// Death detection: Mario rơi xuống đáy map, hoặc animation DIE đã chạy đủ lâu.
	{
		float px, py, pz;
		player->GetPosition(px, py, pz);
		ULONGLONG now = GetTickCount64();

		if (player->GetState() == MARIO_STATE::DIE && marioDieStart == 0)
			marioDieStart = now;

		bool fellOff = (mapHeight > 0 && py > mapHeight);
		if (fellOff && marioDieStart == 0)
		{
			marioDieStart = now;
			player->SetState(MARIO_STATE::DIE);  // Set DIE state ngay để camera stop following
			SoundManager::GetInstance()->StopBGM();
			SoundManager::GetInstance()->PlaySFX(SFX::DIE);
		}

		bool dieSoundDone = (marioDieStart != 0 && now - marioDieStart >= MARIO_DIE_SCENE_DELAY_MS);

		if (dieSoundDone)
		{
			OnMarioDeath();
			return;
		}
	}

	// Update camera để follow mario (chỉ nếu Mario sống)
	if (player->GetState() != MARIO_STATE::DIE)
	{
		float cx, cy, cz;
		player->GetPosition(cx, cy, cz);

		Camera::GetInstance()->Follow(cx, fixedCameraY);
	}

	// Động đất màn cuối: rung camera từng đợt, dừng hẳn khi Mario lên được cầu.
	if (Camera::GetInstance()->IsEarthquakeActive())
	{
		if (IsPlayerOnCastleBridge())
			Camera::GetInstance()->StopEarthquake();
		else
			Camera::GetInstance()->UpdateEarthquake((float)dt);
	}

	// Xóa deleted objects
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

// Render tất cả objects và HUD
void PlayScene::Render()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}

	if (hud != nullptr)
		hud->Render();

	if (menuHUD != nullptr)
		menuHUD->Render();
}

// Kiểm tra Mario có đang đứng trên cầu cuối màn không
bool PlayScene::IsPlayerOnCastleBridge()
{
	if (player == nullptr) return false;

	float ml, mt, mr, mb;
	player->GetBoundingBox(ml, mt, mr, mb);

	for (auto obj : objects)
	{
		auto bridge = dynamic_cast<CastleBridge*>(obj);
		if (bridge == nullptr || bridge->IsDeleted()) continue;

		float bl, bt, br, bb;
		bridge->GetBoundingBox(bl, bt, br, bb);

		// Mario chồng theo phương ngang và chân (đáy) tì lên mặt trên của cầu.
		bool overlapX     = (mr > bl) && (ml < br);
		bool restingOnTop = (mb >= bt - 4.0f) && (mb <= bt + 8.0f);
		if (overlapX && restingOnTop)
			return true;
	}
	return false;
}

// Bắt đầu stage clear sequence
void PlayScene::StartStageClear()
{
	stageClearActive = true;
	stageClearStart = GetTickCount64();
	stageClearTime = hud->GetRemainingTime();  // Thời gian thực tế để tính điểm
	stageClearDisplayTime = 100;  // Debug: hiển thị countdown 100 giây, giảm từng 5 giây

	// Stop BGM hiện tại (bao gồm warning BGM nếu đang chơi)
	SoundManager::GetInstance()->StopBGM();

	// Play stage clear BGM
	SoundManager::GetInstance()->PlayBGM(BGM::STAGE_CLEAR_THEME, false);

	// Disable warning BGM logic trong stage clear
	if (hud != nullptr)
		hud->SetStageClearActive(true);
}

// Xử lý khi Mario chết
void PlayScene::OnMarioDeath()
{
	PlayerData& pd = PlayerData::Get();
	pd.lives--;
	pd.returnScene = id;   // level to resume if lives remain

	// Lives còn lại -> death screen (Enter resume level này).
	// Hết mạng -> game-over screen (Enter replay từ đầu).
	if (pd.lives > 0)
		GameManager::GetInstance()->InitiateSwitchScene(SCENE::DEATH);
	else
		GameManager::GetInstance()->InitiateSwitchScene(SCENE::GAME_OVER);
}

// Unload scene: xóa tất cả objects và HUD
void PlayScene::Unload()
{
	delete hud;
	hud = NULL;

	delete menuHUD;
	menuHUD = NULL;

	for (size_t i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();
	player = NULL;
	menuOptions = NULL;   // được sở hữu bởi `objects`, đã xóa ở trên
}
