#include "Mario.h"
#include "../Graphic/AnimationManager.h"
#include "../Resource/AssetID.h"
#include "../Resource/debug.h"
#include "Bullet.h"
#include "BrickTest.h"
#include "Goomba.h"
#include "Troopa.h"
#include "GameObject.h"
#include "Mushroom.h"
#include "Coin.h"
#include "Star.h"
#include "FireFlower.h"
#include "OneUpMushroom.h"

void Mario::MovementUpdate(DWORD dt) {
	vx += accelX * dt;
	vy += gravity * dt;

	// Friction logic when IDLE
	if (state == MARIO_STATE_IDLE) {
		if (vx > 0) {
			vx -= MARIO_ACCEL_WALK * dt;
			if (vx < 0) vx = 0;
		}
		else if (vx < 0) {
			vx += MARIO_ACCEL_WALK * dt;
			if (vx > 0) vx = 0;
		}
	}
	else if(state == MARIO_STATE_DIE) {
		vx = 0;
	}

	// Max speed capping
	float maxV = MARIO_VELOCITY_WALK_MAX;
	if (state == MARIO_STATE_RUNNING_RIGHT || state == MARIO_STATE_RUNNING_LEFT)
		maxV = MARIO_VELOCITY_RUN_MAX;

	if (abs(vx) > maxV) {
		vx = (vx > 0) ? maxV : -maxV;
	}
}

void Mario::ShootBullet() {
	float bulletX = x + (direction > 0 ? 15.0f : -8.0f);
	float bulletY = y;
	scene->AddObject(new Bullet(bulletX, bulletY, direction, this));
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	MovementUpdate(dt);
	Collision::GetInstance()->Process(this, dt, coObjects);
}

void Mario::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		accelX = MARIO_ACCEL_WALK;
		direction = 1;
		break;

	case MARIO_STATE_WALKING_LEFT:
		accelX = -MARIO_ACCEL_WALK;
		direction = -1;
		break;

	case MARIO_STATE_RUNNING_RIGHT:
		accelX = MARIO_ACCEL_RUN;
		direction = 1;
		break;

	case MARIO_STATE_RUNNING_LEFT:
		accelX = -MARIO_ACCEL_RUN;
		direction = -1;
		break;

	case MARIO_STATE_JUMP:
		if (isOnGround) {
			vy = -MARIO_JUMP_SPEED;
			isOnGround = false;
		}
		break;

	case MARIO_STATE_IDLE:
		accelX = 0;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_SPEED;
		accelX = 0;
		break;

	case MARIO_STATE_SHOOT:
		ShootBullet();
		break;

	case MARIO_STATE_SIT:
		accelX = 0;
		break;
	}
}

void Mario::Render()
{
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	// BẠN CÓ THỂ BỔ SUNG LEVEL FIRE VÀO ĐÂY SAU NẾU CÓ ANIMATION RIÊNG
	else if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
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
	l = x;
	t = y;
	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
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
	if (dynamic_cast<Brick*>(e->obj))
	{
		if (e->ny < 0 && e->obj->IsBlocking()) { 
			vy = 0;
			isOnGround = true;
		}
	}

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
			if (dynamic_cast<Brick*>(e->obj))
			{
				if (e->ny < 0)
				{
					// Landing on top of a brick - snap to surface, stop falling
					y += e->t * vy * e->ny;
					vy = 0;
					isOnGround = true;
				}
				else if (e->ny > 0)
				{
					// Hitting a brick from BELOW - bounce off, keep falling
					// Just zero out upward velocity so Mario falls back down
					vy = 0;
				}
				else if (e->nx != 0)
				{
					// Side collision - stop horizontal movement
					x += e->t * vx * e->nx;
					vx = 0;
				}
				else if (e->ny > 0 && e->obj->IsBlocking()) {
					vy = 0;
				}
			}

			// 1. Va chạm với NẤM ĐỎ (+1000 điểm, Biến lớn)
			else if (dynamic_cast<Mushroom*>(e->obj))
			{
				Mushroom* mushroom = dynamic_cast<Mushroom*>(e->obj);
				if (!mushroom->IsDeleted()) {
					if (this->level == MARIO_LEVEL_SMALL) {
						this->level = MARIO_LEVEL_BIG;
						this->y -= 12.0f; // Tránh lọt đất 
					}
					this->AddScore(1000);
					mushroom->Delete();
				}
			}
			// 2. Va chạm với ĐỒNG XU (+100 điểm)
			else if (dynamic_cast<Coin*>(e->obj))
			{
				Coin* coin = dynamic_cast<Coin*>(e->obj);
				if (!coin->IsDeleted()) {
					this->AddScore(100);
					coin->Delete();
				}
			}
			// 3. Va chạm với HOA LỬA (+1000 điểm, Biến Fire)
			else if (dynamic_cast<FireFlower*>(e->obj))
			{
				FireFlower* flower = dynamic_cast<FireFlower*>(e->obj);
				if (!flower->IsDeleted()) {
					if (this->level == MARIO_LEVEL_SMALL) {
						this->y -= 12.0f; // Vẫn phải trừ 12 để không lọt đất nếu đang nhỏ
					}
					this->level = MARIO_LEVEL_FIRE;
					this->AddScore(1000);
					flower->Delete();
				}
			}
			// 4. Va chạm với NGÔI SAO (+1000 điểm, Vô địch)
			else if (dynamic_cast<Star*>(e->obj))
			{
				Star* star = dynamic_cast<Star*>(e->obj);
				if (!star->IsDeleted()) {
					this->isInvincible = true; // Kích hoạt trạng thái bất tử 
					this->AddScore(1000);
					star->Delete();
				}
			}
			// 5. Va chạm với NẤM XANH (+1 mạng)
			else if (dynamic_cast<OneUpMushroom*>(e->obj))
			{
				OneUpMushroom* upMushroom = dynamic_cast<OneUpMushroom*>(e->obj);
				if (!upMushroom->IsDeleted()) {
					this->lives++; // Tăng thêm 1 mạng 
					upMushroom->Delete();
				}
			}
		}
	}
}

void Mario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}
