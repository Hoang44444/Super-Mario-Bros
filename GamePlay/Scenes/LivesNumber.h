#pragma once
#include "GameObject.h"
#include "AnimationManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "PlayerData.h"
#include "../Resource/AssetID.h"

// Hiển thị số mạng CÒN LẠI (1..3) cạnh chữ X trên màn hình Mario chết (death scene).
// Đọc trực tiếp PlayerData::lives nên luôn đúng số mạng hiện tại.
// (x,y) là toạ độ logic, neo theo camera giống background; (w,h) là kích thước vẽ.
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
		if (lives < 1 || lives > 3) return;                 // ảnh chỉ có số 1..3
		int aniId = ANIMATION::LIVES_DIGIT_1 + (lives - 1); // digit n -> 3020 + (n-1)

		LPANIMATION ani = AnimationManager::GetInstance()->Get(aniId);
		if (ani == nullptr) return;

		float camX = Camera::GetInstance()->GetX();
		float camY = Camera::GetInstance()->GetY();
		ani->RenderScaled(camX + x, camY + y, z, w, h);
	}
};
