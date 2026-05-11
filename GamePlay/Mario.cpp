#include "Mario.h"
#include "../Graphic/AnimationManager.h"
#include "../Resource/AssetID.h"
#include "../Resource/debug.h"
#include "Bullet.h"
#include "BrickTest.h"
#include "Goomba.h"
#include "Troopa.h"
#include "GameObject.h"

void Mario::MovementUpdate(DWORD dt) {
	this->x += this->vx * dt;
	this->y += this->vy * dt;

	this->vx += this->accelX * dt;
	this->vy += this->gravity * dt;
}

void Mario::ShootBullet() {
	float bulletX = x + (direction > 0 ? 15.0f : -8.0f);
	float bulletY = y;
	scene->AddObject(new Bullet(bulletX, bulletY, direction, this));
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	this->vx += this->accelX * dt;
	this->vy += this->gravity * dt;

	if (state == MARIO_STATE_DIE)
	{
		this->x += this->vx * dt;
		this->y += this->vy * dt;
		return;
	}

	isOnGround = false;

	Collision::GetInstance()->Process(this, dt, coObjects);
}

void Mario::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALK_SPEED;
		direction = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALK_SPEED;
		direction = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isOnGround) {
			vy = -MARIO_JUMP_SPEED;
			isOnGround = false;
		}
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_SPEED;
		break;
	case MARIO_STATE_SHOOT:
		ShootBullet();
		this->state = MARIO_STATE_IDLE;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		vx = -MARIO_RUN_SPEED;
		direction = -1;
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		vx = MARIO_RUN_SPEED;
		direction = 1;
		break;
	}
}

void Mario::Render()
{
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
	{
		if (!isOnGround) {
			if (direction > 0) aniId = ID_ANI_MARIO_BIG_JUMP_WALK_RIGHT;
			else aniId = ID_ANI_MARIO_BIG_JUMP_WALK_LEFT;
		}
		else if (vx != 0) {
			if (direction > 0) aniId = ID_ANI_MARIO_BIG_WALKING_RIGHT;
			else aniId = ID_ANI_MARIO_BIG_WALKING_LEFT;
		}
		else {
			if (direction > 0) aniId = ID_ANI_MARIO_BIG_IDLE_RIGHT;
			else aniId = ID_ANI_MARIO_BIG_IDLE_LEFT;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (!isOnGround) {
			if (direction > 0) aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
		else if (vx != 0) {
			if (direction > 0) aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			else aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
		}
		else {
			if (direction > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
			else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
		}
	}

	if (aniId != -1)
	{
		int drawX = (int)round(x);
		int drawY = (int)round(y);
		if (direction > 0) {
			drawX += 1;
		}

		AnimationManager::GetInstance()->Get(aniId)->Render(drawX, drawY);
	}
}

void Mario::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (level == MARIO_LEVEL_BIG)
	{
		l = x + 1.0f;
		r = x + 15.0f;

		t = y;
		b = y + 31.0f;
	}
	else
	{
		l = x + 1.5f;
		r = x + 14.5f;

		t = y;
		b = y + 15.0f;
	}
}

void Mario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == MARIO_STATE_DIE) return;

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnGround = true;
	}
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<Goomba*>(e->obj))
	{
		Goomba* goomba = dynamic_cast<Goomba*>(e->obj);
		if (e->ny < 0) {
			if (goomba->GetState() != GOOMBA_STATE_DIE) {
				goomba->SetState(GOOMBA_STATE_DIE);
				vy = -MARIO_JUMP_SPEED / 1.5f;
			}
		}
		else {
			if (goomba->GetState() != GOOMBA_STATE_DIE) SetState(MARIO_STATE_DIE);
		}
	}

	else if (dynamic_cast<Troopa*>(e->obj))
	{
		Troopa* troopa = dynamic_cast<Troopa*>(e->obj);
		int tState = troopa->GetState();

		if (e->ny < 0)
		{
			if (tState == TROOPA_STATE_SHELL_MOVING)
			{
				SetState(MARIO_STATE_DIE);
			}
			else if (tState == TROOPA_STATE_WALKING)
			{
				troopa->SetState(TROOPA_STATE_SHELL);
				vy = -MARIO_JUMP_SPEED / 1.5f;
				this->y -= 2.0f;
			}
			else if (tState == TROOPA_STATE_SHELL)
			{
				troopa->SetState(TROOPA_STATE_SHELL_MOVING);

				float marioCenter = this->x + 7.0f;
				float troopaCenter = troopa->GetX() + 8.0f;
				float shellVx = (marioCenter < troopaCenter) ? TROOPA_SHELL_SPEED : -TROOPA_SHELL_SPEED;
				troopa->SetVx(shellVx);

				vy = -MARIO_JUMP_SPEED / 1.5f;
				this->y -= 2.0f;
			}
		}
		else if (e->nx != 0 || e->ny > 0)
		{
			if (tState == TROOPA_STATE_SHELL)
			{
				troopa->SetState(TROOPA_STATE_SHELL_MOVING);
				float shellVx = (e->nx < 0) ? TROOPA_SHELL_SPEED : -TROOPA_SHELL_SPEED;
				troopa->SetVx(shellVx);

				troopa->SetPosition(troopa->GetX() + (e->nx * -2.0f), troopa->GetY());
			}
			else
			{
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void Mario::OnNoCollision(DWORD dt)
{
	this->x += this->vx * dt;
	this->y += this->vy * dt;
}

