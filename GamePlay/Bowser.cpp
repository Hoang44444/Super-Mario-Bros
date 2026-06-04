#include "Bowser.h"
#include "BowserFireball.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "PlayScene.h"

Bowser::Bowser(float x, float y, float z) : Enemy(x, y, z)
{
	this->startX = x;
	this->startY = y;
	this->direction = -1;
	this->vx = -BOWSER_WALK_SPEED;
	this->vy = 0;
	this->hp = 5;
	this->lastShootTime = GetTickCount();
	this->lastJumpTime = GetTickCount();
	this->isJumping = false;
}

void Bowser::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += BOWSER_GRAVITY * dt;

	// Patrol movement range
	if (x < startX - BOWSER_WALK_RANGE)
	{
		vx = BOWSER_WALK_SPEED;
	}
	else if (x > startX + BOWSER_WALK_RANGE)
	{
		vx = -BOWSER_WALK_SPEED;
	}

	// Always face Mario
	PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
	if (playScene && playScene->GetPlayer())
	{
		float mx, my, mz;
		playScene->GetPlayer()->GetPosition(mx, my, mz);
		direction = (mx < x) ? -1 : 1;
	}

	// Shooting behavior
	DWORD now = GetTickCount();
	if (now - lastShootTime > BOWSER_SHOOT_INTERVAL)
	{
		ShootFire();
		lastShootTime = now;
	}

	// Jumping behavior
	if (now - lastJumpTime > BOWSER_JUMP_INTERVAL)
	{
		Jump();
		lastJumpTime = now;
	}

	Collision::GetInstance()->Process(this, dt, coObjects);
}

void Bowser::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::BOWSER_WALK)->Render(x, y, z);
}

void Bowser::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BOWSER_WIDTH;
	b = y + BOWSER_HEIGHT;
}

void Bowser::OnMarioCollison(Mario* mario)
{
	mario->SetState(MARIO_STATE_DIE);
}

void Bowser::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Bowser::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0)
		{
			isJumping = false;
		}
	}
}

void Bowser::TakeDamage(int damage)
{
	this->hp -= damage;
	if (this->hp <= 0)
	{
		this->Delete();
	}
}

void Bowser::ShootFire()
{
	PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
	if (playScene)
	{
		// Spawn fireball at mouth level
		BowserFireball* fireball = new BowserFireball(x + (direction * 8.0f), y + 8.0f, z, direction);
		playScene->AddObject(fireball);
	}
}

void Bowser::Jump()
{
	if (!isJumping)
	{
		vy = -BOWSER_JUMP_SPEED;
		isJumping = true;
	}
}