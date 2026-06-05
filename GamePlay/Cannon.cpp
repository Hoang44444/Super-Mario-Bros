#include "Cannon.h"
#include "BulletBill.h"
#include "PlayScene.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
Cannon::Cannon(float x, float y, float z, int dir) : StaticObject(x, y, z)
{
	this->direction = dir;
	this->shootTimer = 0;
}

void Cannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DWORD now = GetTickCount();
	if (now - shootTimer >= CANNON_SHOOT_INTERVAL)
	{
		// Spawn a new BulletBill
		PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
		if (playScene)
		{
			BulletBill* bullet = new BulletBill(x, y, z);
			bullet->SetMovement(direction);
			playScene->AddObject(bullet);
		}
		shootTimer = now;
	}
}

void Cannon::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::CANNON)->Render(x, y, z);
}

void Cannon::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	// No bounding box for Cannon
}