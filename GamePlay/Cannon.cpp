#include "Cannon.h"
#include "BulletBill.h"
#include "PlayScene.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "GameObject.h"

Cannon::Cannon(float x, float y, float z, int dir) : StaticObject(x, y, z)
{
	this->direction = dir;
	this->shootTimer = 0;
}

void Cannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
    if (!playScene) return;

    Mario* mario = dynamic_cast<Mario*>(playScene->GetPlayer());  

    if (!mario) return;

    // Khoảng cách từ Cannon tới Mario
    float marioX, marioY, marioZ;
    mario->GetPosition(marioX, marioY, marioZ);

    float dx = marioX - x;


    // Chỉ bắn khi Mario ở gần (300 pixel)
    if (abs(dx) > 300)
        return;

    // Chỉ bắn đúng phía nòng pháo
    if ((direction == 1 && dx < 0) ||
        (direction == -1 && dx > 0))
        return;

    ULONGLONG now = GetTickCount64();

    if (now - shootTimer >= CANNON_SHOOT_INTERVAL)
    {
        BulletBill* bullet = new BulletBill(x + direction * 16, y, z);
        bullet->SetMovement(direction);
        playScene->AddObject(bullet);

        shootTimer = now;
    }
}

void Cannon::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::CANNON)->Render(x, y, z);
}

void Cannon::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CANNON_BBOX_WIDTH;
	b = y + CANNON_BBOX_HEIGHT;
}