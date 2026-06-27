#include "MenuHUD.h"

#include <cstdio>

#include "AnimationManager.h"
#include "Camera.h"
#include "Renderer.h"
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
		DT_CENTER | DT_TOP | DT_NOCLIP,
		color);
}

void MenuHUD::DrawCursor()
{
	// TODO: Replace with mushroom cursor sprite.
	// For now, draw a simple placeholder using text.
	
	Renderer* r = Renderer::GetInstance();
	float scale = r->GetGlobalScale();
	float logicalW = r->GetBackBufferWidth() / scale;
	float logicalH = r->GetBackBufferHeight() / scale;
	
	float camX = Camera::GetInstance()->GetX();
	float camY = Camera::GetInstance()->GetY();
	
	// Draw a simple ">" as cursor placeholder
	CreateFontIfNeeded();
	if (font != nullptr)
	{
		wchar_t cursor = L'>';
		DrawTextLine(&cursor, (int)(camX + cursorX), (int)(camY + cursorY), 50, 60, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}
}

void MenuHUD::Update()
{
	// Update cursor Y position based on selected option
	// Menu items are spaced vertically
	const float startY = 200.0f;
	const float spacing = 80.0f;
	
	cursorY = startY + selectedOption * spacing;
}

void MenuHUD::Render()
{
	CreateFontIfNeeded();
	if (font == nullptr) return;

	Renderer* r = Renderer::GetInstance();
	float scale = r->GetGlobalScale();
	float logicalW = r->GetBackBufferWidth() / scale;
	float logicalH = r->GetBackBufferHeight() / scale;
	
	float camX = Camera::GetInstance()->GetX();
	float camY = Camera::GetInstance()->GetY();

	// Menu item positions
	const float startX = 200.0f;
	const float startY = 200.0f;
	const float spacing = 80.0f;
	
	// Draw menu items
	const wchar_t* menuItems[] = { L"START", L"LEVEL", L"HELP", L"QUIT" };
	
	for (int i = 0; i < 4; i++)
	{
		D3DXCOLOR color = (i == selectedOption) ? 
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f) :  // Yellow for selected
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   // White for unselected
		
		DrawTextLine(menuItems[i], (int)(camX + startX), (int)(camY + startY + i * spacing), 300, 60, color);
	}
	
	// Draw cursor
	DrawCursor();
}

void MenuHUD::MoveSelection(int delta)
{
	selectedOption += delta;
	
	// Clamp to valid range
	if (selectedOption < 0) selectedOption = 3;
	if (selectedOption > 3) selectedOption = 0;
}
