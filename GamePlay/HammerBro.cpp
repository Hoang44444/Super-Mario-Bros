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
	this->state = HAMMER_BRO_STATE_WALK_LEFT;   // khởi đầu đi sang trái
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

	// Handle throwing hammers: mỗi đợt tung ra 10 cây, KHÔNG cùng lúc mà mỗi cây
	// cách nhau một khoảng thời gian rất nhỏ (HAMMER_BRO_BURST_GAP).
	DWORD now = GetTickCount();
	if (hammersLeft == 0 && now - lastThrowTime > HAMMER_BRO_THROW_INTERVAL)
	{
		hammersLeft = HAMMER_BRO_BURST_COUNT;   // bắt đầu một đợt mới
		lastBurstSpawn = 0;                      // để cây đầu tiên bay ra ngay
	}
	if (hammersLeft > 0 && now - lastBurstSpawn >= HAMMER_BRO_BURST_GAP)
	{
		ThrowHammer();
		lastBurstSpawn = now;
		hammersLeft--;
		if (hammersLeft == 0)
			lastThrowTime = now;   // hết đợt -> tính lại cooldown cho đợt sau
	}

	// Handle jumping
	if (now - lastJumpTime > HAMMER_BRO_JUMP_INTERVAL)
	{
		Jump();
		lastJumpTime = now;
	}

	// Cập nhật trạng thái để quyết định animation: đang nhảy -> JUMP;
	// còn lại thì theo hướng nhìn Mario (trái/phải).
	if (isJumping)
		state = HAMMER_BRO_STATE_JUMP;
	else if (direction < 0)
		state = HAMMER_BRO_STATE_WALK_LEFT;
	else
		state = HAMMER_BRO_STATE_WALK_RIGHT;

	// Physics and collision resolution
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
			isJumping = false; // Reset jumping state when landing on top of a blocking object
		}
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		// Đụng tường/block theo phương ngang -> quạy đầu (đổi hướng di chuyển).
		// ReverseDirection(cooldown) chỉ lật tối đa 1 lần mỗi HAMMER_BRO_TURN_COOLDOWN
		// nhờ lastTurnTime, nên không bị lật liên tục khi còn tì vào tường.
		ReverseDirection(HAMMER_BRO_TURN_COOLDOWN);
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