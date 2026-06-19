#include "HammerBro.h"
#include "Hammer.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "PlayScene.h"

HammerBro::HammerBro(float x, float y, float z) : Enemy(x, y, z)
{
	this->startX = x;
	this->startY = y;
	this->direction = -1;
	this->vx = -HAMMER_BRO_WALK_SPEED;
	this->vy = 0;
	this->lastThrowTime = GetTickCount();
	this->lastJumpTime = GetTickCount();
	this->isJumping = false;
}

void HammerBro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += HAMMER_BRO_GRAVITY * dt;

	// Patrol movement logic
	if (x < startX - HAMMER_BRO_WALK_RANGE)
	{
		vx = HAMMER_BRO_WALK_SPEED;
	}
	else if (x > startX + HAMMER_BRO_WALK_RANGE)
	{
		vx = -HAMMER_BRO_WALK_SPEED;
	}

	// Always face Mario
	PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
	if (playScene && playScene->GetPlayer())
	{
		float mx, my, mz;
		playScene->GetPlayer()->GetPosition(mx, my, mz);
		direction = (mx < x) ? -1 : 1;
	}

	// Handle throwing hammers
	DWORD now = GetTickCount();
	if (now - lastThrowTime > HAMMER_BRO_THROW_INTERVAL)
	{
		ThrowHammer();
		lastThrowTime = now;
	}

	// Handle jumping
	if (now - lastJumpTime > HAMMER_BRO_JUMP_INTERVAL)
	{
		Jump();
		lastJumpTime = now;
	}

	// Physics and collision resolution
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void HammerBro::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::HAMMER_BRO_WALK)->Render(x, y, z);
}

void HammerBro::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + HAMMER_BRO_WIDTH;
	b = y + HAMMER_BRO_HEIGHT;
}

void HammerBro::OnMarioCollison(Mario* mario, float ny)
{
	mario->TakeDamage();
}

void HammerBro::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void HammerBro::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0)
		{
			isJumping = false; // Reset jumping state when landing on top of a blocking object
		}
	}
}

void HammerBro::ThrowHammer()
{
	PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
	if (playScene)
	{
		// Spawn hammer slightly above Hammer Bro
		Hammer* hammer = new Hammer(x + (direction * 4.0f), y - 8.0f, z, direction);
		playScene->AddObject(hammer);
	}
}

void HammerBro::Jump()
{
	if (!isJumping)
	{
		// 50% chance to jump high, or jump low (e.g. drop down)
		vy = -HAMMER_BRO_JUMP_SPEED;
		isJumping = true;
	}
}