#include "Blooper.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "PlayScene.h"

Blooper::Blooper(float x, float y, float z) : Enemy(x, y, z)
{
	// Start in idle state
	this->state = BLOOPER_STATE_IDLE;
	this->timer = GetTickCount();
    this->startY = y;
    this->flipX = false;
    this->chaseDistance = BLOOPER_DETECT_DISTANCE + 50;
}

void Blooper::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DWORD now = GetTickCount();

	// Try to obtain Mario's position if we have a scene reference
	PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
	float marioX = 0.0f, marioY = 0.0f;
	if (playScene && playScene->GetPlayer())
	{
		playScene->GetPlayer()->GetPosition(marioX, marioY, this->z);
	}
    // Compute horizontal distance to Mario
    float distX = abs(marioX - x);
    // If Mario is out of chase range while Blooper is chasing, return to idle
    if (distX > chaseDistance && state != BLOOPER_STATE_IDLE) {
        state = BLOOPER_STATE_IDLE;
        timer = now;
        y = startY;          // reset vertical position
        flipX = false;
        return;
    }

    switch (state)
    {
    case BLOOPER_STATE_IDLE:
        // Check distance to Mario to activate
        if (playScene && playScene->GetPlayer())
        {
            float dist = abs(marioX - x);
            if (dist < chaseDistance && marioY > y)
            {
                state = BLOOPER_STATE_SINKING;
                timer = now;
            }
            else 
                {
                state = BLOOPER_STATE_RISING;
                timer = now;
			}
        }
        break;

    case BLOOPER_STATE_RISING:
        // Move upward
        y -= 0.04f * dt;
        // Horizontal adjustment towards Mario while rising
        if (marioX < x) x -= 0.04f * dt;
        else if (marioX > x) x += 0.04f * dt;
        if (now - timer > BLOOPER_RISE_TIME)
        {
            state = BLOOPER_STATE_SINKING;
            timer = now;
        }
        break;

    case BLOOPER_STATE_SINKING:
        // Move downward
        y += 0.04f * dt;
        // Horizontal chase towards Mario
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

void Blooper::OnMarioCollison(Mario* mario)
{
	mario->SetState(MARIO_STATE_DIE);
}