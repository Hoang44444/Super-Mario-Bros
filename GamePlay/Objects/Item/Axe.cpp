#include "Axe.h"
#include "AnimationManager.h"
#include "PlayScene.h"
#include "AssetID.h"
#include "CastleBridge.h"
#include "Mario.h"

void Axe::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::AXE)->Render(x, y, z);
}

void Axe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x; t = y;
	r = x + AXE_BBOX_WIDTH;
	b = y + AXE_BBOX_HEIGHT;
}

void Axe::OnMarioCollision(Mario* mario)
{
	PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
	if (playScene != nullptr)
	{
		// Làm sập mọi cầu trong scene, có độ trễ để chúng sập theo thứ tự
		DWORD bridgeDelay = 0;
		for (LPGAMEOBJECT obj : playScene->GetObjects())
		{
			CastleBridge* bridge = dynamic_cast<CastleBridge*>(obj);
			if (bridge != nullptr) { bridge->Collapse(bridgeDelay); bridgeDelay += 120; }
		}
	}

	if (mario != nullptr)
		mario->SetState(MARIO_STATE::WALKING_RIGHT);

	Delete();
}
