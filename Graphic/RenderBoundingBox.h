#pragma once

#include <d3d10.h>
#include <d3dx10.h>
#include "Renderer.h"
#include "../Resource/Texture.h"
#include "../GamePlay/GameObject.h"

#include "../Resource/AssetID.h"

class RenderBoundingBox
{
private:
	LPTEXTURE bboxTexture;
	bool isInitialized;

public:
	RenderBoundingBox();
	~RenderBoundingBox();

	void Init(LPCWSTR textureFilePath);
	void Render(LPGAMEOBJECT gameObject);
	void Cleanup();
};
