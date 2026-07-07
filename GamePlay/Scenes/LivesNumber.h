#pragma once
#include "GameObject.h"
#include "AnimationManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "PlayerData.h"
#include "AssetID.h"

class LivesNumber : public GameObject
{
	float w, h;

public:
	LivesNumber(float x, float y, float z, float w, float h)
		: GameObject(x, y, z), w(w), h(h) {}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) override { l = t = r = b = 0; }
	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }

	void Render() override
	{
		int lives = PlayerData::Get().lives;
		if (lives < 1 || lives > 3) return;
		int aniId = ANIMATION::LIVES_DIGIT_1 + (lives - 1);

		LPANIMATION ani = AnimationManager::GetInstance()->Get(aniId);
		if (ani == nullptr) return;

		float camX = Camera::GetInstance()->GetX();
		float camY = Camera::GetInstance()->GetY();
		ani->RenderScaled(camX + x, camY + y, z, w, h);
	}
};
