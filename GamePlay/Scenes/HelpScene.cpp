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

class HelpSceneKeyHandler : public KeyEventHandler
{
private:
	HelpScene* scene;

public:
	HelpSceneKeyHandler(HelpScene* scene) : scene(scene) {}
	void KeyState(BYTE* states) override {}
	void OnKeyDown(int keyCode) override
	{
		switch (keyCode)
		{
		case VK_UP:
			scene->MoveSelection(-1);
			break;
		case VK_DOWN:
			scene->MoveSelection(1);
			break;
		case VK_LEFT:
			scene->AdjustSelection(-10);
			break;
		case VK_RIGHT:
			scene->AdjustSelection(10);
			break;
		case VK_RETURN:
			scene->ConfirmSelection();
			break;
		case VK_ESCAPE:
			scene->BackToMenu();
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
	wchar_t line[128];
	swprintf_s(line, L"%s < %3d >", label, value);

	D3DXCOLOR color = selected ?
		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f) :
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	DrawTextLine(selected ? L">" : L"", x - 42, y, 40, 44, color, DT_LEFT | DT_TOP | DT_NOCLIP);
	DrawTextLine(line, x, y, 520, 44, color, DT_LEFT | DT_TOP | DT_NOCLIP);
}

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

	// UI layout: adjust these values to reposition help text and volume controls.
	const int leftX = 420;
	const int rightX = 840;
	const int controlsY = 170;
	const int lineHeight = 52;
	const int volumeY = 370;

	DrawTextLine(L"Move left / right", leftX, controlsY, 380, 44, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), DT_LEFT | DT_TOP | DT_NOCLIP);
	DrawTextLine(L"Left / Right Arrow", rightX, controlsY, 520, 44, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), DT_LEFT | DT_TOP | DT_NOCLIP);
	DrawTextLine(L"Jump", leftX, controlsY + lineHeight, 380, 44, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), DT_LEFT | DT_TOP | DT_NOCLIP);
	DrawTextLine(L"Space", rightX, controlsY + lineHeight, 520, 44, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), DT_LEFT | DT_TOP | DT_NOCLIP);
	DrawTextLine(L"Shoot", leftX, controlsY + lineHeight * 2, 380, 44, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), DT_LEFT | DT_TOP | DT_NOCLIP);
	DrawTextLine(L"Shift", rightX, controlsY + lineHeight * 2, 520, 44, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), DT_LEFT | DT_TOP | DT_NOCLIP);

	SoundManager* sound = SoundManager::GetInstance();
	DrawVolumeOption(L"Master Volume", sound->GetMasterVolume(), leftX, volumeY, selectedOption == 0);
	DrawVolumeOption(L"Music Volume ", sound->GetMusicVolume(), leftX, volumeY + lineHeight, selectedOption == 1);
	DrawVolumeOption(L"SFX Volume   ", sound->GetSFXVolume(), leftX, volumeY + lineHeight * 2, selectedOption == 2);

	D3DXCOLOR backColor = (selectedOption == 3) ?
		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f) :
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
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

void HelpScene::MoveSelection(int delta)
{
	selectedOption += delta;
	if (selectedOption < 0) selectedOption = 3;
	if (selectedOption > 3) selectedOption = 0;
}

void HelpScene::AdjustSelection(int delta)
{
	AdjustSelectedVolume(delta);
}

void HelpScene::ConfirmSelection()
{
	if (selectedOption == 3)
		BackToMenu();
}

void HelpScene::BackToMenu()
{
	GameManager::GetInstance()->InitiateSwitchScene(SCENE::MENU);
}
