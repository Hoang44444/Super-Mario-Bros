#include "HUD.h"

#include <cstdio>

#include "AnimationManager.h"
#include "Camera.h"
#include "Mario.h"
#include "Renderer.h"
#include "../Resource/AssetID.h"

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

void HUD::ResetTimer()
{
	remainingTime = START_TIME;
	elapsedMs = 0;
}

void HUD::Update(DWORD dt)
{
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
	if (coinAnimation != nullptr)
	{
		const float scale = Renderer::GetInstance()->GetGlobalScale();
		const float coinScreenX = 250.0f / scale;
		const float coinScreenY = 82.0f / scale;

		coinAnimation->Render(
			Camera::GetInstance()->GetX() + coinScreenX,
			Camera::GetInstance()->GetY() + coinScreenY,
			0.0f);
	}

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
	const int coins = ReadCoin(mario, 0);
	const int lives = ReadLives(mario, 0);

	wchar_t line[128];
	DrawTextLine(L"MARIO", 30, 20, 300, 60);
	DrawTextLine(L"WORLD", 500, 20, 300, 60);
	DrawTextLine(L"TIME", 700, 20, 300, 60);

	swprintf_s(line, L"%06d", score);
	DrawTextLine(line, 30, 80, 300, 60);

	swprintf_s(line, L"x%02d", coins);
	DrawTextLine(line, 280, 80, 200, 60);

	swprintf_s(line, L"%d-%d", world, stage);
	DrawTextLine(line, 500, 80, 200, 60);

	swprintf_s(line, L"%03d", remainingTime);
	DrawTextLine(line, 700, 80, 200, 60);

	//swprintf_s(line, L"LIVES %02d", lives);
	//DrawTextLine(line, 30, 140, 400, 60);

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
