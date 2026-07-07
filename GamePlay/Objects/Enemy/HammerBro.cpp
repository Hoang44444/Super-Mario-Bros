#include "HammerBro.h"
#include "Hammer.h"
#include "AnimationManager.h"
#include "AssetID.h"
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
	this->state = HAMMER_BRO_STATE_WALK_LEFT;
}

void HammerBro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += HAMMER_BRO_GRAVITY * dt;

	if (x < startX - HAMMER_BRO_WALK_RANGE)
	{
		vx = HAMMER_BRO_WALK_SPEED;
	}
	else if (x > startX + HAMMER_BRO_WALK_RANGE)
	{
		vx = -HAMMER_BRO_WALK_SPEED;
	}

	PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
	if (playScene && playScene->GetPlayer())
	{
		float mx, my, mz;
		playScene->GetPlayer()->GetPosition(mx, my, mz);
		direction = (mx < x) ? -1 : 1;
	}

	DWORD now = GetTickCount();
	if (hammersLeft == 0 && now - lastThrowTime > HAMMER_BRO_THROW_INTERVAL)
	{
		hammersLeft = HAMMER_BRO_BURST_COUNT;
		lastBurstSpawn = 0;
	}
	if (hammersLeft > 0 && now - lastBurstSpawn >= HAMMER_BRO_BURST_GAP)
	{
		ThrowHammer();
		lastBurstSpawn = now;
		hammersLeft--;
		if (hammersLeft == 0)
			lastThrowTime = now;
	}

	if (now - lastJumpTime > HAMMER_BRO_JUMP_INTERVAL)
	{
		Jump();
		lastJumpTime = now;
	}

	if (isJumping)
		state = HAMMER_BRO_STATE_JUMP;
	else if (direction < 0)
		state = HAMMER_BRO_STATE_WALK_LEFT;
	else
		state = HAMMER_BRO_STATE_WALK_RIGHT;

	Collision::GetInstance()->Process(this, dt, coObjects);
}

void HammerBro::Render()
{
	int aniId;
	switch (state)
	{
	case HAMMER_BRO_STATE_JUMP:
		aniId = ANIMATION::HAMMER_BRO_JUMP;
		break;
	case HAMMER_BRO_STATE_WALK_RIGHT:
		aniId = ANIMATION::HAMMER_BRO_WALK_RIGHT;
		break;
	case HAMMER_BRO_STATE_WALK_LEFT:
	default:
		aniId = ANIMATION::HAMMER_BRO_WALK_LEFT;
		break;
	}

	LPANIMATION ani = AnimationManager::GetInstance()->Get(aniId);
	if (ani != nullptr) ani->Render(x, y, z);
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
			isJumping = false;
		}
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		ReverseDirection(HAMMER_BRO_TURN_COOLDOWN);
	}
}

void HammerBro::ThrowHammer()
{
	PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
	if (playScene)
	{
		Hammer* hammer = new Hammer(x + (direction * 4.0f), y - 8.0f, z, direction);
		playScene->AddObject(hammer);
	}
}

void HammerBro::Jump()
{
	if (!isJumping)
	{
		vy = -HAMMER_BRO_JUMP_SPEED;
		isJumping = true;
	}
}