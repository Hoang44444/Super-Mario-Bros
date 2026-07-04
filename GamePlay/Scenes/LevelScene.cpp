#include "LevelScene.h"

#include <fstream>
#include <sstream>
#include <vector>

#include "Camera.h"
#include "GameManager.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "../Resource/AssetID.h"
#include "../Objects/Player/PlayerData.h"

// Key handler cho LevelScene - xử lý input bàn phím cho menu chọn level
class LevelSceneKeyHandler : public KeyEventHandler
{
private:
	LevelScene* scene;  // Con trỏ đến LevelScene

public:
	LevelSceneKeyHandler(LevelScene* scene) : scene(scene) {}
	void KeyState(BYTE* states) override {}
	void OnKeyDown(int keyCode) override
	{
		switch (keyCode)
		{
		case VK_LEFT:
			scene->MoveSelection(-1);  // Di trái
			break;
		case VK_RIGHT:
			scene->MoveSelection(1);  // Di phải
			break;
		case VK_RETURN:
			scene->ConfirmSelection();  // Xác nhận
			break;
		case VK_ESCAPE:
			scene->BackToMenu();  // Quay về menu
			break;
		}
	}
	void OnKeyUp(int keyCode) override {}
};

LevelScene::LevelScene(int id, LPCWSTR filePath) : Scene(id, filePath)
{
	background = nullptr;
	font = nullptr;
	titleFont = nullptr;
	selectedLevel = 0;
	key_handler = new LevelSceneKeyHandler(this);
}

LevelScene::~LevelScene()
{
	Unload();
	delete key_handler;
	key_handler = nullptr;
}

// Đọc file scene để lấy đường dẫn background
// Nếu không tìm thấy, dùng mặc định
void LevelScene::LoadBackgroundPath()
{
	std::ifstream f(sceneFilePath.c_str());
	if (!f.is_open())
	{
		background = TextureManager::GetInstance()->LoadTexture(L"Texture/Scene/level_scene.png");
		return;
	}

	int section = 0;
	char str[1024];
	while (f.getline(str, 1024))
	{
		std::string line(str);
		if (line.empty() || line[0] == '#') continue;
		if (!line.empty() && line.back() == '\r') line.pop_back();

		if (line == "[BACKGROUND]") { section = 1; continue; }
		if (line[0] == '[') { section = 0; continue; }

		if (section == 1)
		{
			std::stringstream ss(line);
			std::string path;
			ss >> path;
			if (!path.empty())
			{
				std::wstring wpath(path.begin(), path.end());
				background = TextureManager::GetInstance()->LoadTexture(wpath.c_str());
				return;
			}
		}
	}

	background = TextureManager::GetInstance()->LoadTexture(L"Texture/Scene/level_scene.png");
}

void LevelScene::CreateFontIfNeeded()
{
	if (font != nullptr) return;

	ID3D10Device* device = Renderer::GetInstance()->GetDevice();
	if (device == nullptr) return;

	D3DX10CreateFont(
		device,
		54,
		0,
		FW_BOLD,
		1,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE,
		L"Consolas",
		&font);

	D3DX10CreateFont(
		device,
		96,
		0,
		FW_BOLD,
		1,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE,
		L"Consolas",
		&titleFont);
}

// Load background, reset camera
void LevelScene::Load()
{
	LoadBackgroundPath();
	Camera::GetInstance()->SetPosition(0.0f, 0.0f);
}

void LevelScene::Unload()
{
	if (background != nullptr)
	{
		delete background;
		background = nullptr;
	}

	if (font != nullptr)
	{
		font->Release();
		font = nullptr;
	}

	if (titleFont != nullptr)
	{
		titleFont->Release();
		titleFont = nullptr;
	}
}

void LevelScene::Update(DWORD dt)
{
}

// Vẽ background stretch để lấp đầy màn hình
void LevelScene::DrawBackground()
{
	if (background == nullptr) return;

	Renderer* r = Renderer::GetInstance();
	float scale = r->GetGlobalScale();
	float logicalW = r->GetBackBufferWidth() / scale;
	float logicalH = r->GetBackBufferHeight() / scale;
	float camX = Camera::GetInstance()->GetX();
	float camY = Camera::GetInstance()->GetY();

	r->DrawScaled(camX, camY, 0.99f, background, logicalW, logicalH);
}

// Vẽ text với font thường
void LevelScene::DrawTextLine(const wchar_t* text, int x, int y, int width, int height, D3DXCOLOR color, DWORD format)
{
	if (font == nullptr) return;

	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + width;
	rect.bottom = y + height;

	font->DrawText(Renderer::GetInstance()->GetSpriteHandler(), text, -1, &rect, format, color);
}

// Vẽ text với font lớn (title)
void LevelScene::DrawTitleText(const wchar_t* text, int x, int y, int width, int height, D3DXCOLOR color, DWORD format)
{
	if (titleFont == nullptr) return;

	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + width;
	rect.bottom = y + height;

	titleFont->DrawText(Renderer::GetInstance()->GetSpriteHandler(), text, -1, &rect, format, color);
}

// Vẽ text căn giữa màn hình với font lớn
void LevelScene::DrawCenteredText(const wchar_t* text, int y, int height, D3DXCOLOR color)
{
	DrawTitleText(text, 0, y, Renderer::GetInstance()->GetBackBufferWidth(), height, color, DT_CENTER | DT_TOP | DT_NOCLIP);
}

void LevelScene::Render()
{
	CreateFontIfNeeded();

	ID3D10Device* device = Renderer::GetInstance()->GetDevice();
	ID3DX10Sprite* spriteHandler = Renderer::GetInstance()->GetSpriteHandler();
	if (device == nullptr || spriteHandler == nullptr) return;

	D3DXMATRIX oldProjection;
	D3DXMATRIX oldView;
	spriteHandler->GetProjectionTransform(&oldProjection);
	spriteHandler->GetViewTransform(&oldView);

	UINT oldViewportCount = D3D10_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	D3D10_VIEWPORT oldViewports[D3D10_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	device->RSGetViewports(&oldViewportCount, oldViewports);

	ID3D10BlendState* oldBlendState = nullptr;
	FLOAT oldBlendFactor[4] = {};
	UINT oldSampleMask = 0;
	device->OMGetBlendState(&oldBlendState, oldBlendFactor, &oldSampleMask);

	ID3D10SamplerState* oldSamplerState = nullptr;
	device->PSGetSamplers(0, 1, &oldSamplerState);

	DrawBackground();
	spriteHandler->Flush();

	if (font == nullptr)
	{
		spriteHandler->SetProjectionTransform(&oldProjection);
		spriteHandler->SetViewTransform(&oldView);
		device->RSSetViewports(oldViewportCount, oldViewports);
		device->OMSetBlendState(oldBlendState, oldBlendFactor, oldSampleMask);
		device->PSSetSamplers(0, 1, &oldSamplerState);
		if (oldBlendState != nullptr) oldBlendState->Release();
		if (oldSamplerState != nullptr) oldSamplerState->Release();
		return;
	}

	// UI layout: responsive theo kích thước màn hình (original: 440x240 logical)
	Renderer* r = Renderer::GetInstance();
	int screenWidth = r->GetBackBufferWidth();
	int screenHeight = r->GetBackBufferHeight();
	float scale = r->GetGlobalScale();
	float logicalW = screenWidth / scale;
	float logicalH = screenHeight / scale;

	// Original values: titleY=170, levelY=380, startX=270, spacing=310, levelWidth=120, levelHeight=70 (cho 1280x720)
	// Scale factors dựa trên resolution logic 440x240
	float titleY = logicalH * 0.70f;   // ~170/240
	float levelY = logicalH * 1.58f;  // ~380/240
	float startX = logicalW * 0.61f;   // ~270/440
	float spacing = logicalW * 0.70f; // ~310/440
	float levelWidth = logicalW * 0.27f; // ~120/440
	float levelHeight = logicalH * 0.29f; // ~70/240

	// Vẽ title "LEVEL" ở giữa màn hình
	DrawCenteredText(L"LEVEL", (int)titleY, 120, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// Vẽ các level 1-1, 1-2, 1-3, 1-4 với highlight cho level được chọn
	for (int i = 0; i < 4; i++)
	{
		D3DXCOLOR color = (i == selectedLevel) ?
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f) :
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		DrawTextLine(levels[i], (int)(startX + spacing * i), (int)levelY, (int)levelWidth, (int)levelHeight, color, DT_CENTER | DT_TOP | DT_NOCLIP);
	}

	// Vẽ cursor ">" bên trái level được chọn
	spriteHandler->Flush();

	spriteHandler->SetProjectionTransform(&oldProjection);
	spriteHandler->SetViewTransform(&oldView);
	device->RSSetViewports(oldViewportCount, oldViewports);
	device->OMSetBlendState(oldBlendState, oldBlendFactor, oldSampleMask);
	device->PSSetSamplers(0, 1, &oldSamplerState);

	if (oldBlendState != nullptr)
		oldBlendState->Release();

	if (oldSamplerState != nullptr)
		oldSamplerState->Release();
}

void LevelScene::AddObject(LPGAMEOBJECT obj)
{
}

// Di chuyển lựa chọn với wrap-around (0 <-> 3)
void LevelScene::MoveSelection(int delta)
{
	selectedLevel += delta;
	if (selectedLevel < 0) selectedLevel = 3;
	if (selectedLevel > 3) selectedLevel = 0;
}

// Xác nhận: set returnScene và chuyển qua INTRO scene
void LevelScene::ConfirmSelection()
{
	const int levelScenes[] = {
		SCENE::WORLD_1_1,
		SCENE::WORLD_1_2,
		SCENE::WORLD_1_3,
		SCENE::WORLD_1_4
	};

	// Set returnScene thành level được chọn, rồi đi qua INTRO scene
	PlayerData::Get().returnScene = levelScenes[selectedLevel];
	GameManager::GetInstance()->InitiateSwitchScene(SCENE::INTRO);
}

// Quay về menu chính
void LevelScene::BackToMenu()
{
	GameManager::GetInstance()->InitiateSwitchScene(SCENE::MENU);
}
