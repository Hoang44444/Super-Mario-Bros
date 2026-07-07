#include "LevelScene.h"

#include <fstream>
#include <sstream>
#include <vector>

#include "Camera.h"
#include "GameManager.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "AssetID.h"
#include "PlayerData.h"

class LevelSceneKeyHandler : public KeyEventHandler
{
private:
	LevelScene* scene;

public:
	LevelSceneKeyHandler(LevelScene* scene) : scene(scene) {}
	void KeyState(BYTE* states) override {}
	void OnKeyDown(int keyCode) override
	{
		switch (keyCode)
		{
		case VK_LEFT:
			scene->MoveSelection(-1);
			break;
		case VK_RIGHT:
			scene->MoveSelection(1);
			break;
		case VK_RETURN:
		case VK_SPACE:
			scene->ConfirmSelection();
			break;
		case VK_ESCAPE:
			scene->BackToMenu();
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

	Renderer* r = Renderer::GetInstance();
	int screenWidth = r->GetBackBufferWidth();
	int screenHeight = r->GetBackBufferHeight();
	float scale = r->GetGlobalScale();
	float logicalW = screenWidth / scale;
	float logicalH = screenHeight / scale;

	float titleY = logicalH * 0.70f;
	float levelY = logicalH * 1.58f;
	float startX = logicalW * 0.61f;
	float spacing = logicalW * 0.70f;
	float levelWidth = logicalW * 0.27f;
	float levelHeight = logicalH * 0.29f;

	DrawCenteredText(L"LEVEL", (int)titleY, 120, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	const wchar_t* levels[] = { L"1-1", L"1-2", L"1-3", L"1-4" };
	for (int i = 0; i < 4; i++)
	{
		D3DXCOLOR color = (i == selectedLevel) ?
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f) :
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		DrawTextLine(levels[i], (int)(startX + spacing * i), (int)levelY, (int)levelWidth, (int)levelHeight, color, DT_CENTER | DT_TOP | DT_NOCLIP);
	}

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

void LevelScene::MoveSelection(int delta)
{
	selectedLevel += delta;
	if (selectedLevel < 0) selectedLevel = 3;
	if (selectedLevel > 3) selectedLevel = 0;
}

void LevelScene::ConfirmSelection()
{
	const int levelScenes[] = {
		SCENE::WORLD_1_1,
		SCENE::WORLD_1_2,
		SCENE::WORLD_1_3,
		SCENE::WORLD_1_4
	};

	PlayerData::Get().returnScene = levelScenes[selectedLevel];
	GameManager::GetInstance()->InitiateSwitchScene(SCENE::INTRO);
}

void LevelScene::BackToMenu()
{
	GameManager::GetInstance()->InitiateSwitchScene(SCENE::MENU);
}
