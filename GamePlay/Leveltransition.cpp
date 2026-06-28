#include "LevelTransition.h"
#include "Renderer.h"
#include <cstdio>

LevelTransition::~LevelTransition()
{
	if (font != nullptr)
	{
		font->Release();
		font = nullptr;
	}
}

void LevelTransition::CreateFontIfNeeded()
{
	if (font != nullptr) return;

	ID3D10Device* device = Renderer::GetInstance()->GetDevice();
	if (device == nullptr) return;

	D3DX10CreateFont(
		device,
		60,
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

void LevelTransition::DrawCentered(const wchar_t* text, int y, int fontSize)
{
	if (font == nullptr) return;

	int screenWidth = Renderer::GetInstance()->GetBackBufferWidth();

	RECT rect;
	rect.left = 0;
	rect.top = y;
	rect.right = screenWidth;
	rect.bottom = y + fontSize + 20;

	font->DrawText(
		Renderer::GetInstance()->GetSpriteHandler(),
		text,
		-1,
		&rect,
		DT_CENTER | DT_TOP | DT_NOCLIP,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

void LevelTransition::Start(int world, int stage, int lives)
{
	this->world = world;
	this->stage = stage;
	this->lives = lives;
	this->elapsed = 0;
}

bool LevelTransition::Update(DWORD dt)
{
	elapsed += dt;
	return elapsed >= DURATION_MS;
}

void LevelTransition::Render()
{
	CreateFontIfNeeded();
	if (font == nullptr) return;

	ID3D10Device* device = Renderer::GetInstance()->GetDevice();
	if (device == nullptr) return;

	ID3DX10Sprite* spriteHandler = Renderer::GetInstance()->GetSpriteHandler();
	if (spriteHandler == nullptr) return;

	// ID3DX10Font::DrawText repoints the shared sprite handler's
	// projection/view transform (and other pipeline state) to screen-space
	// text mode. If we don't restore it, every world-space sprite drawn by
	// later scenes (background, Mario, tiles...) inherits that transform
	// and renders squashed. Save everything first, exactly like HUD::Render does.
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

	spriteHandler->Flush();

	int screenHeight = Renderer::GetInstance()->GetBackBufferHeight();
	int centerY = screenHeight / 2;

	wchar_t line[64];
	swprintf_s(line, L"WORLD  %d-%d", world, stage);
	DrawCentered(line, centerY - 80, 60);

	swprintf_s(line, L"x %d", lives);
	DrawCentered(line, centerY + 10, 40);

	spriteHandler->Flush();
	spriteHandler->SetProjectionTransform(&oldProjection);
	spriteHandler->SetViewTransform(&oldView);
	device->RSSetViewports(oldViewportCount, oldViewports);
	device->OMSetBlendState(oldBlendState, oldBlendFactor, oldSampleMask);
	device->PSSetSamplers(0, 1, &oldSamplerState);

	if (oldBlendState != nullptr) oldBlendState->Release();
	if (oldSamplerState != nullptr) oldSamplerState->Release();
}