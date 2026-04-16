#include "PlayScene.h"
#include "BrickTest.h"
#include "TextureManager.h"

void PlayScene::Load()
{
	// Load the brick texture
	TextureManager::GetInstance()->Add(0, L"..\\Application\\brick.ico");
	LPTEXTURE brickTex = TextureManager::GetInstance()->Get(0);
	if (brickTex == NULL)
		OutputDebugString(L"[ERROR] Texture NULL\n");
	else
		OutputDebugString(L"[OK] Texture loaded!\n");

	// Create a brick that moves between x=50 and x=400
	Brick* brick = new Brick(100.0f, 100.0f, 50.0f, 400.0f, 0.1f);
	brick->SetTexture(brickTex);

	objects.push_back(brick);

	wchar_t path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
	OutputDebugString(path);
	OutputDebugString(L"\n");
}

void PlayScene::Update(DWORD dt)
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt);
	}
}

void PlayScene::Render()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}
}

void PlayScene::Unload()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();
}
