#include "MenuHUD.h"

#include <cstdio>

#include "AnimationManager.h"
#include "Camera.h"
#include "Renderer.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "../Resource/AssetID.h"

MenuHUD::MenuHUD()
{
	this->font = nullptr;
	this->selectedOption = 0;
	
	// Cursor initial position (will be updated in Update)
	this->cursorX = 150.0f;
	this->cursorY = 200.0f;
}

MenuHUD::~MenuHUD()
{
	if (font != nullptr)
	{
		font->Release();
		font = nullptr;
	}
}

void MenuHUD::CreateFontIfNeeded()
{
	if (font != nullptr) return;

	ID3D10Device* device = Renderer::GetInstance()->GetDevice();
	if (device == nullptr) return;

	D3DX10CreateFont(
		device,
		60,  // Height
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

void MenuHUD::DrawTextLine(const wchar_t* text, int x, int y, int width, int height, D3DXCOLOR color)
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
		color);
}

void MenuHUD::DrawCursor()
{
	Renderer* r = Renderer::GetInstance();
	float scale = r->GetGlobalScale();
	
	float camX = Camera::GetInstance()->GetX();
	float camY = Camera::GetInstance()->GetY();
	
	LPSPRITE cursorSprite = SpriteManager::GetInstance()->Get(11);
	if (cursorSprite != nullptr)
	{
		RECT rect = cursorSprite->GetRect();
		r->Draw(
			camX + cursorX / scale,
			camY + cursorY / scale,
			0.9f,
			cursorSprite->GetTexture(),
			&rect
		);
	}
}

void MenuHUD::Update()
{
	Renderer* r = Renderer::GetInstance();
	int screenWidth = r->GetBackBufferWidth();
	int screenHeight = r->GetBackBufferHeight();
	float scale = r->GetGlobalScale();

	float startY = screenHeight * 0.45f;
	float spacing = 80.0f;

	cursorX = screenWidth * 0.65f - 70.0f;
	cursorY = startY + selectedOption * spacing + (60.0f - 16.0f * scale) / 2.0f;
}

void MenuHUD::Render()
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

	UINT oldViewportCount = D3D10_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	D3D10_VIEWPORT oldViewports[D3D10_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	device->RSGetViewports(&oldViewportCount, oldViewports);

	ID3D10BlendState* oldBlendState = nullptr;
	FLOAT oldBlendFactor[4] = {};
	UINT oldSampleMask = 0;
	device->OMGetBlendState(&oldBlendState, oldBlendFactor, &oldSampleMask);

	ID3D10SamplerState* oldSamplerState = nullptr;
	device->PSGetSamplers(0, 1, &oldSamplerState);

	// 1. Flush background
	spriteHandler->Flush();

	Renderer* r = Renderer::GetInstance();
	int screenWidth = r->GetBackBufferWidth();
	int screenHeight = r->GetBackBufferHeight();
	float scale = r->GetGlobalScale();
	
	float camX = Camera::GetInstance()->GetX();
	float camY = Camera::GetInstance()->GetY();

	// 2. Render Mario decoration (world-space)
	LPANIMATION marioAni = AnimationManager::GetInstance()->Get(400); // MARIO_BIG_IDLE_RIGHT
	if (marioAni != nullptr)
	{
		float marioX = screenWidth * 0.25f;
		float marioY = screenHeight * 0.55f;
		marioAni->Render(
			camX + marioX / scale,
			camY + marioY / scale,
			0.5f
		);
	}

	// 3. Render Cursor (world-space)
	DrawCursor();

	// 4. Flush Mario and Cursor sprites
	spriteHandler->Flush();

	// 5. Draw menu items (screen-space)
	float startX = screenWidth * 0.65f;
	float startY = screenHeight * 0.45f;
	float spacing = 80.0f;
	
	const wchar_t* menuItems[] = { L"START", L"LEVEL", L"HELP", L"QUIT" };
	
	for (int i = 0; i < 4; i++)
	{
		D3DXCOLOR color = (i == selectedOption) ? 
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f) :  // Yellow for selected
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   // White for unselected
		
		DrawTextLine(menuItems[i], (int)startX, (int)(startY + i * spacing), 300, 60, color);
	}

	// 6. Flush text drawings
	spriteHandler->Flush();

	// 7. Restore original transforms and pipeline state
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

void MenuHUD::MoveSelection(int delta)
{
	selectedOption += delta;
	
	// Clamp to valid range
	if (selectedOption < 0) selectedOption = 3;
	if (selectedOption > 3) selectedOption = 0;
}
