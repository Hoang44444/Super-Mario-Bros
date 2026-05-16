#include "SwitchScenePoint.h"
#include "Mario.h"
#include "GameManager.h"
#include "debug.h"

void SwitchScenePoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void SwitchScenePoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 32.0f;
	b = y + 32.0f;
}

void SwitchScenePoint::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<Mario*>(e->obj))
		OnCollisionWithMario(e);
}

void SwitchScenePoint::OnNoCollision(DWORD dt)
{

}

void SwitchScenePoint::OnCollisionWithMario(LPCOLLISIONEVENT e)
{
	DebugOut(L"[INFO] Mario collided with SwitchScenePoint at (%f, %f)\n", x, y);
	//int currentSceneID = GameManager::GetInstance()->GetCurrentSceneID();
	//int nextSceneID = (currentSceneID + 1) % 2; 
	//GameManager::GetInstance()->InitiateSwitchScene(nextSceneID);
}