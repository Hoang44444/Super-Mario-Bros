#include "Blooper.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "PlayScene.h"

Blooper::Blooper(float x, float y, float z) : Enemy(x, y, z)
{
	this->state = BLOOPER_STATE_IDLE;
	this->timer = GetTickCount();
    this->startY = y;
    this->flipX = false;
    this->chaseDistance = BLOOPER_DETECT_DISTANCE + 50;
}

void Blooper::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DWORD now = GetTickCount();

	PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
	float marioX = 0.0f, marioY = 0.0f;
	if (playScene && playScene->GetPlayer())
	{
		playScene->GetPlayer()->GetPosition(marioX, marioY, this->z);
	}
    float distX = abs(marioX - x);
    if (distX > chaseDistance && state != BLOOPER_STATE_IDLE) {
        state = BLOOPER_STATE_IDLE;
        timer = now;
        y = startY;
        flipX = false;
        return;
    }

    switch (state)
    {
    case BLOOPER_STATE_IDLE:
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
        y -= 0.04f * dt;
        if (marioX < x) x -= 0.04f * dt;
        else if (marioX > x) x += 0.04f * dt;
        if (now - timer > BLOOPER_RISE_TIME)
        {
            state = BLOOPER_STATE_SINKING;
            timer = now;
        }
        break;

    case BLOOPER_STATE_SINKING:
        y += 0.04f * dt;
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