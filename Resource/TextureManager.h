#pragma once
#include <unordered_map>
#include <d3dx10.h>

#include "Texture.h"

using namespace std;

class TextureManager
{
	static TextureManager* __instance;

	unordered_map<int, LPTEXTURE> textures;

public:
	TextureManager();
	void Add(int id, LPCWSTR filePath);
	LPTEXTURE Get(unsigned int i);
	void Clear();
	LPTEXTURE LoadTexture(LPCWSTR texturePath);
	static TextureManager* GetInstance();
};