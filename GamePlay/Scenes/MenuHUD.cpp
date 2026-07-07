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
	
	// Vị trí con trỏ ban đầu (sẽ được cập nhật trong Update)
	this->cursorX = 150.0f;
	this->cursorY = 200.0f;

	this->menuStartX = 0.0f;
	this->menuStartY = 0.0f;
	this->menuSpacing = 0.0f;
	this->fontHeight = 0.0f;
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
		80,  // Chiều cao font (tăng từ 60 lên 80 để chữ lớn hơn)
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

	menuStartX = screenWidth * 0.70f;
	menuStartY = screenHeight * 0.30f;
	menuSpacing = 100.0f;
	fontHeight = 80.0f;

	LPSPRITE cursorSprite = SpriteManager::GetInstance()->Get(11);
	float cursorWidth = 16.0f;
	float cursorHeight = 16.0f;
	if (cursorSprite != nullptr)
	{
		RECT rect = cursorSprite->GetRect();
		cursorWidth = (float)(rect.right - rect.left);
		cursorHeight = (float)(rect.bottom - rect.top);
	}

	// Đặt con trỏ bên trái menu
	cursorX = menuStartX - (cursorWidth * scale) - 20.0f;
	// Căn giữa con trỏ theo chiều dọc so với dòng text
	cursorY = menuStartY + selectedOption * menuSpacing + (fontHeight - cursorHeight * scale) / 2.0f;
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

	// 1. Xóa background
	spriteHandler->Flush();

	Renderer* r = Renderer::GetInstance();
	int screenWidth = r->GetBackBufferWidth();
	int screenHeight = r->GetBackBufferHeight();
	float scale = r->GetGlobalScale();
	
	float camX = Camera::GetInstance()->GetX();
	float camY = Camera::GetInstance()->GetY();

	// 2. Vẽ Mario trang trí (world-space)
	LPANIMATION marioAni = AnimationManager::GetInstance()->Get(1100); // MARIO_SMALL_IDLE_RIGHT
	if (marioAni != nullptr)
	{
		float marioX = screenWidth * 0.15f;
		float marioY = screenHeight * 0.80f;
		marioAni->Render(
			camX + marioX / scale,
			camY + marioY / scale,
			0.5f
		);
	}

	// 3. Vẽ con trỏ chọn (world-space)
	DrawCursor();

	// 4. Xóa sprite Mario và con trỏ
	spriteHandler->Flush();

	// 5. Vẽ các tùy chọn menu (screen-space)
	const wchar_t* menuItems[] = { L"START", L"LEVEL", L"HELP", L"QUIT" };
	
	for (int i = 0; i < 4; i++)
	{
		D3DXCOLOR color = (i == selectedOption) ? 
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f) :  // Vàng cho tùy chọn đang chọn
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   // Trắng cho tùy chọn khác
		
		DrawTextLine(menuItems[i], (int)menuStartX, (int)(menuStartY + i * menuSpacing), 400, (int)fontHeight, color);
	}

	// 6. Xóa text
	spriteHandler->Flush();

	// 7. Khôi phục trạng thái ban đầu
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
	
	// Giới hạn trong khoảng 0-3 (vòng lại)
	if (selectedOption < 0) selectedOption = 3;
	if (selectedOption > 3) selectedOption = 0;
}
