#include "Blooper.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "PlayScene.h"

Blooper::Blooper(float x, float y, float z) : Enemy(x, y, z)
{
	// Start ở idle state
	this->state = BLOOPER_STATE_IDLE;
	this->timer = GetTickCount();
    this->startY = y;
    this->flipX = false;
    this->chaseDistance = BLOOPER_DETECT_DISTANCE + 50;
}

void Blooper::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DWORD now = GetTickCount();

	// Tìm vị trí Mario nếu có scene reference
	PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
	float marioX = 0.0f, marioY = 0.0f;
	if (playScene && playScene->GetPlayer())
	{
		playScene->GetPlayer()->GetPosition(marioX, marioY, this->z);
	}
    // Tính khoảng cách ngang đến Mario
    float distX = abs(marioX - x);
    // Nếu Mario ra khỏi phạm vi đuổi theo khi Blooper đang đuổi, quay về idle
    if (distX > chaseDistance && state != BLOOPER_STATE_IDLE) {
        state = BLOOPER_STATE_IDLE;
        timer = now;
        y = startY;          // reset vị trí dọc
        flipX = false;
        return;
    }

    switch (state)
    {
    case BLOOPER_STATE_IDLE:
        // Check khoảng cách đến Mario để kích hoạt
        if (playScene && playScene->GetPlayer())
        {
            float dist = abs(marioX - x);
            if (dist < chaseDistance)
            {
                if (marioY > y)
                    state = BLOOPER_STATE_SINKING;
                else
                    state = BLOOPER_STATE_RISING;

                timer = now;
            }
        }
        break;

    case BLOOPER_STATE_RISING:
        // Di chuyển lên
        y -= 0.04f * dt;
        // Điều chỉnh ngang về phía Mario khi đang lên
        if (marioX < x) x -= 0.04f * dt;
        else if (marioX > x) x += 0.04f * dt;
        if (now - timer > BLOOPER_RISE_TIME)
        {
            state = BLOOPER_STATE_SINKING;
            timer = now;
        }
        break;

    case BLOOPER_STATE_SINKING:
        // Di chuyển xuống
        y += 0.04f * dt;
        // Đuổi theo theo chiều ngang
        if (marioX < x) x -= 0.04f * dt;
        else if (marioX > x) x += 0.04 * dt;
        if (now - timer > BLOOPER_SINK_TIME)
        {
            state = BLOOPER_STATE_RISING;
            timer = now;
        }
        break;
    }
}

void Blooper::Render()
{
	switch (state)
	{
	case BLOOPER_STATE_RISING:
		AnimationManager::GetInstance()->Get(ANIMATION::BLOOPER_RISING)->Render(x, y, z, flipX);
		break;
	case BLOOPER_STATE_SINKING:
		AnimationManager::GetInstance()->Get(ANIMATION::BLOOPER_SINKING)->Render(x, y, z, flipX);
		break;
	default:
		AnimationManager::GetInstance()->Get(ANIMATION::BLOOPER_IDLE)->Render(x, y, z, flipX);
		break;
	}
}

void Blooper::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BLOOPER_WIDTH;
	b = y + BLOOPER_HEIGHT;
}

void Blooper::OnMarioCollison(Mario* mario, float ny)
{
	mario->TakeDamage();
}