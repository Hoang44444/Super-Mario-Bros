#pragma once
#include "GameObject.h"
#include "AnimationManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "PlayerData.h"
#include "AssetID.h"

// Hiá»ƒn thá»‹ sá»‘ máº¡ng CÃ’N Láº I (1..3) cáº¡nh chá»¯ X trÃªn mÃ n hÃ¬nh Mario cháº¿t (death scene).
// Äá»c trá»±c tiáº¿p PlayerData::lives nÃªn luÃ´n Ä‘Ãºng sá»‘ máº¡ng hiá»‡n táº¡i.
// (x,y) lÃ  toáº¡ Ä‘á»™ logic, neo theo camera giá»‘ng background; (w,h) lÃ  kÃ­ch thÆ°á»›c váº½.
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
		if (lives < 1 || lives > 3) return;                 // áº£nh chá»‰ cÃ³ sá»‘ 1..3
		int aniId = ANIMATION::LIVES_DIGIT_1 + (lives - 1); // digit n -> 3020 + (n-1)

		LPANIMATION ani = AnimationManager::GetInstance()->Get(aniId);
		if (ani == nullptr) return;

		float camX = Camera::GetInstance()->GetX();
		float camY = Camera::GetInstance()->GetY();
		ani->RenderScaled(camX + x, camY + y, z, w, h);
	}
};
