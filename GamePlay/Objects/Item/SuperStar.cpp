#include "SuperStar.h"
#include "Mario.h"
#include "AnimationManager.h"
#include "AssetID.h"
#include "StaticObject.h"
#include "SoundManager.h"
#include "../Core/ScoreManager.h"

void SuperStar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (UpdateEmerge(dt)) return;
	vy += SUPER_STAR_GRAVITY * dt;
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void SuperStar::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::SUPER_STAR)->Render(x, y, z);
}

void SuperStar::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x; t = y;
	r = x + SUPER_STAR_BBOX_WIDTH;
	b = y + SUPER_STAR_BBOX_HEIGHT;
}

void SuperStar::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void SuperStar::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<Mario*>(e->obj))
	{
		OnMarioCollision(dynamic_cast<Mario*>(e->obj));
	}
	else if (dynamic_cast<StaticObject*>(e->obj))
	{
		if (e->ny < 0) vy = -SUPER_STAR_BOUNCE_SPEED;
		else if (e->ny > 0) vy = 0;
		else if (e->nx != 0) { direction = -direction; vx = SUPER_STAR_SPEED * direction; }
	}
}

void SuperStar::OnMarioCollision(Mario* mario)
{
	SoundManager::GetInstance()->PlaySFX(SFX::POWERUP);
	mario->SetStarPower(MARIO_PARAMS::STAR_POWER_TIME);

	ScoreManager::Get().AddScore(SCORE_VALUES::STARMAN);

	Delete();
}
