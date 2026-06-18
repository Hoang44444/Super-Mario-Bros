#pragma once
#include "GameObject.h"
#include "AnimationManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "MenuKeyHandler.h"
#include "../Resource/AssetID.h"

// On-screen menu text: draws the two options ("Bắt đầu" and "Hướng dẫn").
// The option currently selected in MenuKeyHandler is drawn in its yellow
// variant, the other in its white variant.
class MenuOptions : public GameObject
{
public:
	MenuOptions(float x, float y, float z) : GameObject(x, y, z) {}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) override { l = t = r = b = 0; }
	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }

	void Render() override
	{
		// Native sub-rect sizes inside menu_text.png (to keep aspect ratio).
		const float START_W = 236.0f, START_H = 51.0f;
		const float GUIDE_W = 334.0f, GUIDE_H = 62.0f;
		// Drawn height and logical-Y (top) of each option.
		const float TARGET_H = 24.0f;
		const float START_Y = 130.0f;
		const float GUIDE_Y = 165.0f;

		int selected = 0;
		if (scene != nullptr) {
			MenuKeyHandler* mh = dynamic_cast<MenuKeyHandler*>(scene->GetKeyEventHandler());
			if (mh != nullptr) selected = mh->GetSelected();
		}

		Renderer* r = Renderer::GetInstance();
		float scale = r->GetGlobalScale();
		float logicalW = r->GetBackBufferWidth() / scale;
		float camX = Camera::GetInstance()->GetX();
		float camY = Camera::GetInstance()->GetY();

		DrawOption(selected == 0 ? ANIMATION::MENU_START_YELLOW : ANIMATION::MENU_START_WHITE,
			START_W, START_H, TARGET_H, START_Y, camX, camY, logicalW);
		DrawOption(selected == 1 ? ANIMATION::MENU_GUIDE_YELLOW : ANIMATION::MENU_GUIDE_WHITE,
			GUIDE_W, GUIDE_H, TARGET_H, GUIDE_Y, camX, camY, logicalW);
	}

private:
	// Draw one option scaled to targetH and centered horizontally on screen.
	void DrawOption(int aniId, float srcW, float srcH, float targetH, float targetY,
		float camX, float camY, float logicalW)
	{
		float w = srcW * (targetH / srcH);
		float x = camX + (logicalW - w) / 2.0f;   // center horizontally
		float y = camY + targetY;
		LPANIMATION ani = AnimationManager::GetInstance()->Get(aniId);
		if (ani != nullptr)
			ani->RenderScaled(x, y, z, w, targetH);
	}
};
