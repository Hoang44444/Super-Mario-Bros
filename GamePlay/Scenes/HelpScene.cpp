#include "HelpScene.h"

#include <cwchar>
#include <fstream>
#include <sstream>

#include "Camera.h"
#include "GameManager.h"
#include "Renderer.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "../Resource/AssetID.h"

// Key handler cho HelpScene - xử lý input bàn phím cho menu settings
class HelpSceneKeyHandler : public KeyEventHandler
{
private:
	HelpScene* scene;  // Con trỏ đến HelpScene

public:
	HelpSceneKeyHandler(HelpScene* scene) : scene(scene) {}
	void KeyState(BYTE* states) override {}
	void OnKeyDown(int keyCode) override
	{
		switch (keyCode)
		{
		case VK_UP:
			scene->MoveSelection(-1);  // Di lên
			break;
		case VK_DOWN:
			scene->MoveSelection(1);  // Di xuống
			break;
		case VK_LEFT:
			scene->AdjustSelection(-10);  // Giảm giá trị
			break;
		case VK_RIGHT:
			scene->AdjustSelection(10);  // Tăng giá trị
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

HelpScene::HelpScene(int id, LPCWSTR filePath) : Scene(id, filePath)
{
	background = nullptr;
	font = nullptr;
	selectedOption = 0;
	key_handler = new HelpSceneKeyHandler(this);
}

HelpScene::~HelpScene()
{
	Unload();
	delete key_handler;
	key_handler = nullptr;
}

// Đọc file scene để lấy đường dẫn background
// Nếu không tìm thấy, dùng mặc định
void HelpScene::LoadBackgroundPath()
{
	std::ifstream f(sceneFilePath.c_str());
	if (!f.is_open())
	{
		background = TextureManager::GetInstance()->LoadTexture(L"Texture/Scene/help_scene.png");
		return;
	}

	int section = 0;
	char str[1024];
	while (f.getline(str, 1024))
	{
		std::string line(str);
		if (line.empty() || line[0] == '#') continue;
		if (!line.empty() && line.back() == '\r') line.pop_back();

	// Parse file scene: tìm section [BACKGROUND] và đường dẫn ảnh
	if (line == "[BACKGROUND]") { section = 1; continue; }  // Bắt đầu section background
	if (line[0] == '[') { section = 0; continue; }  // Section khác -> thoát

		if (section == 1)
		{
			// Đọc đường dẫn background từ file
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

	background = TextureManager::GetInstance()->LoadTexture(L"Texture/Scene/help_scene.png");
}

void HelpScene::CreateFontIfNeeded()
{
	if (font != nullptr) return;

	ID3D10Device* device = Renderer::GetInstance()->GetDevice();
	if (device == nullptr) return;

	D3DX10CreateFont(
		device,
		34,
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

void HelpScene::Load()
{
	// Load background, reset camera, áp dụng volume settings
	LoadBackgroundPath();
	Camera::GetInstance()->SetPosition(0.0f, 0.0f);
	ApplyVolumes();
}

void HelpScene::Unload()
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
}

void HelpScene::Update(DWORD dt)
{
}

// Vẽ background stretch để lấp đầy màn hình
void HelpScene::DrawBackground()
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

void HelpScene::DrawTextLine(const wchar_t* text, int x, int y, int width, int height, D3DXCOLOR color, DWORD format)
{
	if (font == nullptr) return;

	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + width;
	rect.bottom = y + height;

	font->DrawText(Renderer::GetInstance()->GetSpriteHandler(), text, -1, &rect, format, color);
}

void HelpScene::DrawVolumeOption(const wchar_t* label, int value, int x, int y, bool selected)
{
	// Vẽ option volume với label, value và highlight nếu được chọn
	// Format: "< value >" với màu vàng
	wchar_t line[128];
	swprintf_s(line, L"%s < %3d >", label, value);

	D3DXCOLOR color = selected ?
		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f) :
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	DrawTextLine(selected ? L">" : L"", x - 42, y, 40, 44, color, DT_LEFT | DT_TOP | DT_NOCLIP);
	DrawTextLine(line, x, y, 520, 44, color, DT_LEFT | DT_TOP | DT_NOCLIP);
}

// Điều chỉnh volume của option được chọn (master/music/sfx)
// Giới hạn trong khoảng [0, 100]
void HelpScene::AdjustSelectedVolume(int delta)
{
	SoundManager* sound = SoundManager::GetInstance();
	int masterVolume = sound->GetMasterVolume();
	int musicVolume = sound->GetMusicVolume();
	int sfxVolume = sound->GetSFXVolume();

	int* value = nullptr;
	if (selectedOption == 0) value = &masterVolume;
	else if (selectedOption == 1) value = &musicVolume;
	else if (selectedOption == 2) value = &sfxVolume;
	else return;

	*value += delta;
	if (*value < 0) *value = 0;
	if (*value > 100) *value = 100;

	sound->SetVolumeSettings(masterVolume, musicVolume, sfxVolume);
}

void HelpScene::ApplyVolumes()
{
	// Áp dụng volume settings: tính effective volume = master * setting / 100
	// Áp dụng cho BGM và tất cả SFX
	SoundManager* sound = SoundManager::GetInstance();
	int masterVolume = sound->GetMasterVolume();
	int musicVolume = sound->GetMusicVolume();
	int sfxVolume = sound->GetSFXVolume();

	int effectiveMusic = (masterVolume * musicVolume) / 100;
	int effectiveSfx = (masterVolume * sfxVolume) / 100;

	SoundManager::GetInstance()->SetBGMVolume(effectiveMusic);

	const int sfxIds[] = {
		SFX::JUMP,
		SFX::JUMP_SMALL,
		SFX::STOMP,
		SFX::COIN,
		SFX::POWERUP,
		SFX::POWERUP_APPEARS,
		SFX::FIREBALL,
		SFX::SHRINK,
		SFX::DIE,
		SFX::ONE_UP,
		SFX::BRICK_BREAK,
		SFX::BUMP,
		SFX::QUESTION_BLOCK,
		SFX::PIPE_WARP,
		SFX::FLAGPOLE,
		SFX::CASTLE_CLEAR,
		SFX::LEVEL_CLEAR,
		SFX::MENU_SELECT,
		SFX::MENU_MOVE,
		SFX::PAUSE,
		SFX::GAME_OVER
	};

	for (int id : sfxIds)
		SoundManager::GetInstance()->SetSFXVolume(id, effectiveSfx);
}

void HelpScene::Render()
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

	// Layout UI: responsive theo kích thước màn hình (original: 440x240 logical)
	// Scale factors dựa trên resolution logic 440x240
	Renderer* r = Renderer::GetInstance();
	int screenWidth = r->GetBackBufferWidth();
	int screenHeight = r->GetBackBufferHeight();
	float scale = r->GetGlobalScale();
	int logicalW = (int)(screenWidth / scale);
	int logicalH = (int)(screenHeight / scale);

	// Original values: leftX=420, rightX=840, controlsY=170, lineHeight=52, volumeY=370 (cho 1280x720)
	// Scale factors dựa trên resolution logic 440x240
	const int leftX = (int)(logicalW * 0.95f);   // ~420/440
	const int rightX = (int)(logicalW * 1.90f);  // ~840/440
	const int controlsY = (int)(logicalH * 0.70f); // ~170/240
	const int lineHeight = (int)(logicalH * 0.22f);  // ~52/240
	const int volumeY = (int)(logicalH * 1.54f);   // ~370/240

	// Vẽ hướng dẫn điều khiển (bên trái: action, bên phải: phím)
	DrawTextLine(L"Left / Right Arrow", rightX, controlsY, 520, 44, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), DT_LEFT | DT_TOP | DT_NOCLIP);
	DrawTextLine(L"Jump", leftX, controlsY + lineHeight, 380, 44, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), DT_LEFT | DT_TOP | DT_NOCLIP);
	DrawTextLine(L"Space", rightX, controlsY + lineHeight, 520, 44, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), DT_LEFT | DT_TOP | DT_NOCLIP);
	DrawTextLine(L"Shoot", leftX, controlsY + lineHeight * 2, 380, 44, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), DT_LEFT | DT_TOP | DT_NOCLIP);
	DrawTextLine(L"Shift", rightX, controlsY + lineHeight * 2, 520, 44, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), DT_LEFT | DT_TOP | DT_NOCLIP);

	SoundManager* sound = SoundManager::GetInstance();
	DrawVolumeOption(L"Master Volume", sound->GetMasterVolume(), leftX, volumeY, selectedOption == 0);
	DrawVolumeOption(L"Music Volume ", sound->GetMusicVolume(), leftX, volumeY + lineHeight, selectedOption == 1);
	DrawVolumeOption(L"SFX Volume   ", sound->GetSFXVolume(), leftX, volumeY + lineHeight * 2, selectedOption == 2);

// Vẽ nút Back với highlight nếu được chọn
	D3DXCOLOR backColor = (selectedOption == 3) ?
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) :
		D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	DrawTextLine(L"<", leftX - 42, volumeY + lineHeight * 3, 40, 44, backColor, DT_LEFT | DT_TOP | DT_NOCLIP);
	DrawTextLine(L"Back", leftX, volumeY + lineHeight * 3, 200, 44, backColor, DT_LEFT | DT_TOP | DT_NOCLIP);

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

void HelpScene::AddObject(LPGAMEOBJECT obj)
{
}

// Di chuyển lựa chọn với wrap-around (0 <-> 3)
void HelpScene::MoveSelection(int delta)
{
	selectedOption += delta;
	if (selectedOption < 0) selectedOption = 3;
	if (selectedOption > 3) selectedOption = 0;
}

// Điều chỉnh giá trị của option hiện tại (volume)
void HelpScene::AdjustSelection(int delta)
{
	AdjustSelectedVolume(delta);
}

void HelpScene::ConfirmSelection()
{
	// Xác nhận: nếu chọn Back thì quay về menu
	if (selectedOption == 3)
		BackToMenu();
}

// Quay về menu chính
void HelpScene::BackToMenu()
{
	GameManager::GetInstance()->InitiateSwitchScene(SCENE::MENU);
}
