#include "Goomba.h"
#include "Koopa.h"
#include "BuzzyBeetle.h"
#include "Camera.h"
#include "AnimationManager.h"
#include "AssetID.h"
#include "debug.h"
#include "ScoreManager.h"

Goomba::Goomba(float x, float y, float z) : Enemy(x, y, z)
{
    this->die_start = 0;
    SetState(GOOMBA_STATE_WALKING);
}

void Goomba::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x;
    t = y;
    r = l + GOOMBA_BBOX_WIDTH;

    if (state == GOOMBA_STATE_DIE)
    {
        b = t + GOOMBA_BBOX_HEIGHT_DIE;
    }
    else
    {
        b = t + GOOMBA_BBOX_HEIGHT;
    }
}

void Goomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state == GOOMBA_STATE_DIE_OTHER)
    {
        vy += GOOMBA_GRAVITY * dt;

        x += vx * dt;
        y += vy * dt;

        float cy = Camera::GetInstance()->GetY();
        const float SCREEN_HEIGHT = 600.0f;
        const float CULL_BUFFER = 100.0f;

        if (this->y > cy + SCREEN_HEIGHT + CULL_BUFFER)
        {
            this->Delete();
        }

        return;
    }

    vy += GOOMBA_GRAVITY * dt;

    if (state == GOOMBA_STATE_DIE && GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT)
    {
        this->Delete();
        return;
    }

    if (state != GOOMBA_STATE_DIE)
    {
        Collision::GetInstance()->Process(this, dt, coObjects);
    }
}

bool Goomba::IsCollidable()
{
    if (state == GOOMBA_STATE_DIE || state == GOOMBA_STATE_DIE_OTHER)
    {
        return false;
    }

    return true;
}

void Goomba::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void Goomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<Koopa*>(e->obj))
    {
        Koopa* koopa = dynamic_cast<Koopa*>(e->obj);

        if (koopa->GetState() == KOOPA_STATE_SPINNING)
        {
            this->SetState(GOOMBA_STATE_DIE_OTHER);
            ScoreManager::Get().AddScore(SCORE_VALUES::SHELL_KICK);
            return;
        }
    }
    else if (dynamic_cast<BuzzyBeetle*>(e->obj))
    {
        BuzzyBeetle* beetle = dynamic_cast<BuzzyBeetle*>(e->obj);

        if (beetle->GetState() == BUZZY_STATE_SPINNING)
        {
            this->SetState(GOOMBA_STATE_DIE_OTHER);
            ScoreManager::Get().AddScore(SCORE_VALUES::SHELL_KICK);
            return;
        }
    }

    if (e->ny != 0 && e->obj->IsBlocking())
    {
        vy = 0;
    }
    else if (e->nx != 0 && e->obj->IsBlocking())
    {
        vx = -vx;
    }
}

void Goomba::OnMarioCollison(Mario* mario, float ny)
{
    if (mario->GetState() == MARIO_STATE::DIE) return;
    if (state == GOOMBA_STATE_DIE || state == GOOMBA_STATE_DIE_OTHER) return;

    if (ny < 0)
    {
        SetState(GOOMBA_STATE_DIE);
        float mvx, mvy;
        mario->GetSpeed(mvx, mvy);
        mario->SetSpeed(mvx, -0.4f);

        ScoreManager& scoreMgr = ScoreManager::Get();
        scoreMgr.IncrementCombo();

        if (scoreMgr.IsMaxCombo())
        {
            mario->AddLife(1);
        }
        else
        {
            mario->AddScore(scoreMgr.GetComboScore());
        }
    }
    else
    {
        mario->TakeDamage();
    }
}

void Goomba::SetState(int state)
{
    GameObject::SetState(state);
    switch (state)
    {
    case GOOMBA_STATE_DIE:
        die_start = GetTickCount64();
        vx = 0;
        vy = 0;

        y += 5.0f;
        break;
    case GOOMBA_STATE_WALKING:
        vx = -GOOMBA_WALKING_SPEED;
        break;
    case GOOMBA_STATE_DIE_OTHER:
        vx = 0;
        vy = -0.3f;
        break;
    }
}

void Goomba::Render()
{
    int aniId = ANIMATION::GOOMBA_WALK;

    if (state == GOOMBA_STATE_DIE)
    {
        aniId = ANIMATION::GOOMBA_DIE;
    }

    AnimationManager::GetInstance()->Get(aniId)->Render(x, y, z);
}