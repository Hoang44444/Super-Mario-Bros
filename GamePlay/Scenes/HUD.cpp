#include "HUD.h"

#include <cstdio>

#include "AnimationManager.h"
#include "Camera.h"
#include "Mario.h"
#include "Renderer.h"
#include "../Resource/AssetID.h"
#include "GameManager.h"
#include "PlayerData.h"

namespace
{
	template <typename T>
	auto ReadScore(T* mario, int) -> decltype(mario->GetScore())
	{
		return mario != nullptr ? mario->GetScore() : 0;
	}

	int ReadScore(...)
	{
		return 0;
	}

	template <typename T>
	auto ReadCoin(T* mario, int) -> decltype(mario->GetCoin())
	{
		return mario != nullptr ? mario->GetCoin() : 0;
	}

	int ReadCoin(...)
	{
		return 0;
	}

	template <typename T>
	auto ReadLives(T* mario, int) -> decltype(mario->GetLives())
	{
		return mario != nullptr ? mario->GetLives() : 0;
	}

	template <typename T>
	auto ReadLives(T* mario, long) -> decltype(mario->GetLife())
	{
		return mario != nullptr ? mario->GetLife() : 0;
	}

	int ReadLives(...)
	{
		return 0;
	}
}

HUD::HUD(Mario* mario, int world, int stage)
{
	this->mario = mario;
	this->world = world;
	this->stage = stage;
	this->remainingTime = START_TIME;
	this->elapsedMs = 0;
	this->font = nullptr;
}

HUD::~HUD()
{
	if (font != nullptr)
	{
		font->Release();
		font = nullptr;
	}
}

void HUD::CreateFontIfNeeded()
{
	if (font != nullptr) return;

	ID3D10Device* device = Renderer::GetInstance()->GetDevice();
	if (device == nullptr) return;

	D3DX10CreateFont(
		device,
		50,
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
}

void HUD::DrawTextLine(const wchar_t* text, int x, int y, int width, int height)
{
	if (font == nullptr) return;

	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + width;
	rect.bottom = y + height;

	font->DrawText(
		Renderer::GetInstance()->GetSpriteHandler(),
		text,
		-1,
		&rect,
		DT_LEFT | DT_TOP | DT_NOCLIP,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

void HUD::DrawCenteredTextLine(const wchar_t* text, int y, int height)
{
	if (font == nullptr) return;

	int screenWidth = Renderer::GetInstance()->GetBackBufferWidth();
	RECT rect;
	rect.left = 0;
	rect.top = y;
	rect.right = screenWidth;
	rect.bottom = y + height;

	font->DrawText(
		Renderer::GetInstance()->GetSpriteHandler(),
		text,
		-1,
		&rect,
		DT_CENTER | DT_TOP | DT_NOCLIP,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

void HUD::RenderMarioAndLives(int lives, int y)
{
	Renderer* r = Renderer::GetInstance();
	float scale = r->GetGlobalScale();
	int screenWidth = r->GetBackBufferWidth();

	float marioWidth = 16.0f * scale; // Small Mario width on screen
	float spacing = 15.0f;
	float textWidth = 60.0f; // Approx width of "x3"
	float totalWidth = marioWidth + spacing + textWidth;

	float startX = (screenWidth - totalWidth) / 2.0f;

	// Render Mario animation (world-space)
	LPANIMATION marioAni = AnimationManager::GetInstance()->Get(1100); // MARIO_SMALL_IDLE_RIGHT
	if (marioAni != nullptr)
	{
		float camX = Camera::GetInstance()->GetX();
		float camY = Camera::GetInstance()->GetY();
		
		marioAni->Render(
			camX + (startX + marioWidth / 2.0f) / scale,
			camY + (y + marioWidth / 2.0f) / scale,
			0.0f
		);
	}

	// Render text
	wchar_t line[16];
	swprintf_s(line, L"x%d", lives);
	DrawTextLine(line, (int)(startX + marioWidth + spacing), y, 200, 60);
}

void HUD::ResetTimer()
{
	remainingTime = START_TIME;
	elapsedMs = 0;
}

void HUD::Update(DWORD dt)
{
	int sceneId = GameManager::GetInstance()->GetCurrentSceneID();
	bool isGameplayScene = (sceneId != SCENE::MENU && 
	                        sceneId != SCENE::CONTROL && 
	                        sceneId != SCENE::DEATH && 
	                        sceneId != SCENE::GAME_OVER && 
	                        sceneId != SCENE::END &&
	                        sceneId != SCENE::INTRO);

	if (!isGameplayScene)
	{
		return;
	}

	if (remainingTime <= 0)
	{
		remainingTime = 0;
		elapsedMs = 0;
		return;
	}

	elapsedMs += dt;
	while (elapsedMs >= 1000 && remainingTime > 0)
	{
		remainingTime--;
		elapsedMs -= 1000;
	}

	if (remainingTime < 0)
		remainingTime = 0;
}

void HUD::Render()
{
	int sceneId = GameManager::GetInstance()->GetCurrentSceneID();
	if (sceneId == SCENE::GAME_OVER) return;

	CreateFontIfNeeded();
	if (font == nullptr) return;

	ID3D10Device* device = Renderer::GetInstance()->GetDevice();
	if (device == nullptr) return;

	ID3DX10Sprite* spriteHandler = Renderer::GetInstance()->GetSpriteHandler();
	if (spriteHandler == nullptr) return;

	D3DXMATRIX oldProjection;
	D3DXMATRIX oldView;
	spriteHandler->GetProjectionTransform(&oldProjection);
	spriteHandler->GetViewTransform(&oldView);

	LPANIMATION coinAnimation = AnimationManager::GetInstance()->Get(ANIMATION::ITEM_COIN);
	
	UINT oldViewportCount = D3D10_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	D3D10_VIEWPORT oldViewports[D3D10_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	device->RSGetViewports(&oldViewportCount, oldViewports);

	ID3D10BlendState* oldBlendState = nullptr;
	FLOAT oldBlendFactor[4] = {};
	UINT oldSampleMask = 0;
	device->OMGetBlendState(&oldBlendState, oldBlendFactor, &oldSampleMask);

	ID3D10SamplerState* oldSamplerState = nullptr;
	device->PSGetSamplers(0, 1, &oldSamplerState);

	// The world is buffered through the same sprite helper. Flush it before
	// ID3DX10Font changes sprite transforms/state for screen-space text.
	spriteHandler->Flush();

	const int score = ReadScore(mario, 0);
	const int coins = (mario != nullptr) ? ReadCoin(mario, 0) : PlayerData::Get().coins;
	const int lives = (mario != nullptr) ? ReadLives(mario, 0) : PlayerData::Get().lives;

	Renderer* r = Renderer::GetInstance();
	int screenWidth = r->GetBackBufferWidth();
	int screenHeight = r->GetBackBufferHeight();
	float scale = r->GetGlobalScale();

	float col0_X = screenWidth * 0.05f;
	float col1_X = screenWidth * 0.25f;
	float col2_X = screenWidth * 0.45f;
	float col3_X = screenWidth * 0.65f;
	float col4_X = screenWidth * 0.85f;

	sceneId = GameManager::GetInstance()->GetCurrentSceneID();
	bool isGameplayScene = (sceneId != SCENE::MENU && 
	                        sceneId != SCENE::CONTROL && 
	                        sceneId != SCENE::DEATH && 
	                        sceneId != SCENE::GAME_OVER && 
	                        sceneId != SCENE::END &&
	                        sceneId != SCENE::INTRO);

	// 1. Draw top row (Labels)
	DrawTextLine(L"SCORE", (int)col0_X, 20, 200, 60);
	DrawTextLine(L"COINS", (int)col1_X, 20, 200, 60);
	DrawTextLine(L"WORLD", (int)col2_X, 20, 200, 60);
	DrawTextLine(L"TIME", (int)col3_X, 20, 200, 60);
	DrawTextLine(L"LIVES", (int)col4_X, 20, 200, 60);

	// 2. Draw values (Row 2)
	wchar_t line[128];
	swprintf_s(line, L"%06d", score);
	DrawTextLine(line, (int)col0_X, 80, 200, 60);

	// Coin Animation
	if (coinAnimation != nullptr)
	{
		const float coinScreenX = col1_X / scale;
		const float coinScreenY = 82.0f / scale;

		coinAnimation->Render(
			Camera::GetInstance()->GetX() + coinScreenX,
			Camera::GetInstance()->GetY() + coinScreenY,
			0.0f);
	}
	swprintf_s(line, L"x%02d", coins);
	DrawTextLine(line, (int)(col1_X + 24.0f * scale), 80, 200, 60);

	swprintf_s(line, L"%d-%d", world, stage);
	DrawTextLine(line, (int)col2_X, 80, 200, 60);

	if (isGameplayScene)
	{
		swprintf_s(line, L"%03d", remainingTime);
		DrawTextLine(line, (int)col3_X, 80, 200, 60);
	}

	swprintf_s(line, L"x%d", lives);
	DrawTextLine(line, (int)col4_X, 80, 200, 60);

	// 3. Render Center Screen Content for INTRO / DEATH
	if (sceneId == SCENE::INTRO)
	{
		// Render WORLD <world>-<stage>
		swprintf_s(line, L"WORLD %d-%d", world, stage);
		DrawCenteredTextLine(line, (int)(screenHeight * 0.5f - 80.0f), 60);

		// Render Mario small and x<lives>
		RenderMarioAndLives(lives, (int)(screenHeight * 0.5f + 20.0f));
	}
	else if (sceneId == SCENE::DEATH)
	{
		// Render Mario small and x<lives> centered
		RenderMarioAndLives(lives, (int)(screenHeight * 0.5f - 30.0f));
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
