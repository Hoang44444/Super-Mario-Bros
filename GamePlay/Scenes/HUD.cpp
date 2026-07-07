#include "HUD.h"

#include <cstdio>

#include "Camera.h"
#include "Mario.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "AssetID.h"
#include "GameManager.h"
#include "PlayerData.h"
#include "ScoreManager.h"
#include "SoundManager.h"
#include "debug.h"

namespace
{
	RECT MakeRect(int left, int top, int right, int bottom)
	{
		RECT rect;
		rect.left = left;
		rect.top = top;
		rect.right = right;
		rect.bottom = bottom;
		return rect;
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
	this->warningBGMPlaying = false;
	this->stageClearOverride = false;
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

void HUD::RenderTextureRectAtScreen(int textureId, const RECT& rect, float screenX, float screenY, float z)
{
	Renderer* r = Renderer::GetInstance();
	float scale = r->GetGlobalScale();
	if (scale <= 0.0f) return;

	LPTEXTURE texture = TextureManager::GetInstance()->Get(textureId);
	if (texture == nullptr) return;

	RECT drawRect = rect;
	r->Draw(
		Camera::GetInstance()->GetX() + screenX / scale,
		Camera::GetInstance()->GetY() + screenY / scale,
		z,
		texture,
		&drawRect);
}

void HUD::RenderCoinIcon(float screenX, float screenY)
{
	const RECT coinFrames[] = {
		MakeRect(300, 98, 316, 115),
		MakeRect(316, 98, 332, 115),
		MakeRect(331, 98, 347, 115)
	};
	int frame = (int)((GetTickCount64() / 100) % 3);
	RenderTextureRectAtScreen(TEXTURE::MISC, coinFrames[frame], screenX, screenY);
}

float HUD::GetMarioLivesStartX() const
{
	Renderer* r = Renderer::GetInstance();
	float scale = r->GetGlobalScale();
	int screenWidth = r->GetBackBufferWidth();

	float marioWidth = 13.0f * scale;
	float spacing = 15.0f;
	float textWidth = 60.0f; // Approx width of "x3"
	float totalWidth = marioWidth + spacing + textWidth;

	return (screenWidth - totalWidth) / 2.0f;
}

void HUD::RenderMarioLivesIcon(int y)
{
	float startX = GetMarioLivesStartX();
	RECT smallMarioIdleRight = MakeRect(247, 0, 261, 16);
	RenderTextureRectAtScreen(TEXTURE::MARIO, smallMarioIdleRight, startX, (float)y);
}

void HUD::DrawMarioLivesText(int lives, int y)
{
	Renderer* r = Renderer::GetInstance();
	float scale = r->GetGlobalScale();

	float marioWidth = 13.0f * scale;
	float spacing = 15.0f;
	float startX = GetMarioLivesStartX();

	wchar_t line[16];
	swprintf_s(line, L"x%d", lives);
	DrawTextLine(line, (int)(startX + marioWidth + spacing), y, 200, 60);
}

void HUD::ResetTimer()
{
	remainingTime = START_TIME;
	elapsedMs = 0;
	warningBGMPlaying = false;
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

	// Timer warning BGM: play when time <= 100s
	// Skip this logic during stage clear (stageClearOverride = true)
	if (!stageClearOverride)
	{
		if (remainingTime <= 100 && !warningBGMPlaying)
		{
			DebugOut(L"[WARNING_BGM_DEBUG] Time <= 100s (%d), playing WARNING_THEME\n", remainingTime);
			SoundManager::GetInstance()->PlayBGM(BGM::WARNING_THEME, true);
			warningBGMPlaying = true;
		}
		else if (remainingTime > 100 && warningBGMPlaying)
		{
			// Time went back above 100 (shouldn't happen normally, but handle it)
			DebugOut(L"[WARNING_BGM_DEBUG] Time > 100s (%d), stopping WARNING_THEME\n", remainingTime);
			SoundManager::GetInstance()->StopBGM();
			warningBGMPlaying = false;
		}
	}

	if (remainingTime <= 0)
	{
		remainingTime = 0;
		elapsedMs = 0;

		// Háº¿t giá» -> Mario cháº¿t. Chá»‰ kÃ­ch hoáº¡t má»™t láº§n (frame vá»«a vá» 0); cÃ¡c frame
		// sau bá»‹ cháº·n bá»Ÿi nhÃ¡nh remainingTime <= 0 á»Ÿ Ä‘áº§u hÃ m.
		if (mario != nullptr && mario->GetState() != MARIO_STATE::DIE)
			mario->SetState(MARIO_STATE::DIE);
	}
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

	const int score = ScoreManager::Get().GetScore();
	const int coins = ScoreManager::Get().GetCoins();
	const int lives = ScoreManager::Get().GetLives();

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

	int marioLivesY = 0;
	if (sceneId == SCENE::INTRO)
		marioLivesY = (int)(screenHeight * 0.5f + 20.0f);
	else if (sceneId == SCENE::DEATH)
		marioLivesY = (int)(screenHeight * 0.5f - 30.0f);

	RenderCoinIcon(col1_X-20, 80.0f);
	if (marioLivesY != 0)
		RenderMarioLivesIcon(marioLivesY);
	spriteHandler->Flush();

	// 1. Draw top row (Labels)
	DrawTextLine(L"SCORE", (int)col0_X, 20, 200, 60);
	DrawTextLine(L"COINS", (int)col1_X, 20, 200, 60);
	DrawTextLine(L"WORLD", (int)col2_X, 20, 200, 60);
	DrawTextLine(L"TIME", (int)col3_X, 20, 200, 60);
	DrawTextLine(L"LIVES", (int)col4_X, 20, 200, 60);

	// 2. Draw values (Row 2)
	wchar_t line[128];
	swprintf_s(line, L"%06d", score);
	DrawTextLine(line, (int)col0_X-10, 80, 200, 60);

	swprintf_s(line, L"x%02d", coins);
	DrawTextLine(line, (int)(col1_X + 40), 80, 200, 60);

	swprintf_s(line, L"%d-%d", world, stage);
	DrawTextLine(line, (int)col2_X + 25, 80, 200, 60);

	if (isGameplayScene)
	{
		swprintf_s(line, L"%03d", remainingTime);
		DrawTextLine(line, (int)col3_X, 80, 200, 60);
	}

	swprintf_s(line, L"x%d", lives);
	DrawTextLine(line, (int)col4_X + 35, 80, 200, 60);

	// 3. Render Center Screen Content for INTRO / DEATH
	if (sceneId == SCENE::INTRO)
	{
		// Render WORLD <world>-<stage>
		swprintf_s(line, L"WORLD %d-%d", world, stage);
		DrawCenteredTextLine(line, (int)(screenHeight * 0.5f - 80.0f), 60);

		// Render Mario small and x<lives>
		DrawMarioLivesText(lives, marioLivesY);
	}
	else if (sceneId == SCENE::DEATH)
	{
		// Render Mario small and x<lives> centered
		DrawMarioLivesText(lives, marioLivesY);
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
