#include "RenderBoundingBox.h"
#include "../GamePlay/GameObject.h"
#include "Camera.h"
#include "debug.h"

RenderBoundingBox::RenderBoundingBox()
	: bboxTexture(NULL), isInitialized(false)
{
}

RenderBoundingBox::~RenderBoundingBox()
{
	Cleanup();
}

void RenderBoundingBox::Init(LPCWSTR textureFilePath)
{
	if (isInitialized) return;

	Renderer* renderer = Renderer::GetInstance();
	bboxTexture = renderer->GetTexture(textureFilePath);

	if (bboxTexture == NULL)
	{
		DebugOut(L"[ERROR] Failed to load bounding box texture: %s\n", textureFilePath);
		return;
	}

	isInitialized = true;
	DebugOut(L"[INFO] RenderBoundingBox initialized successfully with texture: %s\n", textureFilePath);
}

void RenderBoundingBox::Render(LPGAMEOBJECT gameObject)
{
	if (!isInitialized || gameObject == NULL || bboxTexture == NULL)
	{
		return;
	}

	float l, t, r, b;
	gameObject->GetBoundingBox(l, t, r, b);

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)(r - l);
	rect.bottom = (int)(b - t);

	float x, y;
	gameObject->GetPosition(x, y);

	// Draw bounding box with 25% alpha to see through
	// Renderer::Draw handles camera transformation internally
	Renderer::GetInstance()->Draw(x, y, bboxTexture, &rect, 0.25f);
}

void RenderBoundingBox::Cleanup()
{
	if (bboxTexture != NULL)
	{
		delete bboxTexture;
		bboxTexture = NULL;
	}

	isInitialized = false;
}
