#include "Koopa.h"
#include "BuzzyBeetle.h"
#include "Camera.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"
#include "Core/ScoreManager.h"

Koopa::Koopa(float x, float y, float z) : Enemy(x, y, z)
{
    isFacingRight = false;
    SetState(KOOPA_STATE_WALKING);
}

void Koopa::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x;
    t = y;
    r = l + KOOPA_BBOX_WIDTH;

    if (state == KOOPA_STATE_WALKING)
        b = t + KOOPA_BBOX_HEIGHT;
    else
        b = t + KOOPA_BBOX_HEIGHT_SHELL;
}

void Koopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (this->vx > 0) isFacingRight = true;
    else if (this->vx < 0) isFacingRight = false;

    if (state == KOOPA_STATE_DIE)
    {
        vy += KOOPA_GRAVITY * dt;

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

    vy += KOOPA_GRAVITY * dt;
    if (state == KOOPA_STATE_SHELL && GetTickCount64() - shell_start > KOOPA_SHELL_TIMEOUT)
    {
        SetState(KOOPA_STATE_WALKING);

        y -= (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_SHELL);
    }

    Collision::GetInstance()->Process(this, dt, coObjects);
}

bool Koopa::IsCollidable()
{
    if (state == KOOPA_STATE_DIE) return false;
    return true;
}

void Koopa::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void Koopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<Koopa*>(e->obj))
    {
        Koopa* koopa = dynamic_cast<Koopa*>(e->obj);
        if (koopa->GetState() == KOOPA_STATE_SPINNING)
        {
            this->SetState(KOOPA_STATE_DIE);
            // Award điểm cho shell kill (không combo cho shell kills)
            ScoreManager::Get().AddScore(SCORE_VALUES::SHELL_KICK);
            return;
        }
    }
    else if (dynamic_cast<BuzzyBeetle*>(e->obj))
    {
        BuzzyBeetle* beetle = dynamic_cast<BuzzyBeetle*>(e->obj);
        if (beetle->GetState() == BUZZY_STATE_SPINNING)
        {
            this->SetState(KOOPA_STATE_DIE);
            // Award điểm cho shell kill (không combo cho shell kills)
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

void Koopa::OnMarioCollison(Mario* mario, float ny)
{
    if (mario->GetState() == MARIO_STATE_DIE) return;

    if (state == KOOPA_STATE_DIE) return;

    float mx, my, mz;
    mario->GetPosition(mx, my, mz);
    float mvx, mvy;
    mario->GetSpeed(mvx, mvy);

    if (state == KOOPA_STATE_WALKING)
    {
        if (ny < 0)
        {
            SetState(KOOPA_STATE_SHELL);
            mario->SetSpeed(mvx, -0.4f);

            // Award điểm với combo system cho stomp
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

    else if (state == KOOPA_STATE_SHELL)
    {
        if (mx < this->x)
            vx = KOOPA_SPIN_SPEED;
        else
            vx = -KOOPA_SPIN_SPEED;

        SetState(KOOPA_STATE_SPINNING);

        if (ny < 0) mario->SetSpeed(mvx, -0.4f);
    }
    else if (state == KOOPA_STATE_SPINNING)
    {
        if (ny < 0)
        {
            SetState(KOOPA_STATE_SHELL);
            mario->SetSpeed(mvx, -0.4f);
        }
        else
        {
            mario->TakeDamage();
        }
    }
}

void Koopa::SetState(int state)
{
    int oldState = this->state;

    GameObject::SetState(state);

    switch (state)
    {
    case KOOPA_STATE_WALKING:
        vx = -KOOPA_WALK_SPEED;
        vy = 0;
        break;

    case KOOPA_STATE_SHELL:
        shell_start = GetTickCount64();

        vx = 0;
        vy = 0;

        if (oldState == KOOPA_STATE_WALKING)
        {
            y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_SHELL);
        }
        break;

    case KOOPA_STATE_SPINNING:
        break;
    case KOOPA_STATE_DIE:
        vx = 0;
        vy = -0.3f;
        break;
    }
}

void Koopa::Render()
{
    int aniId = ANIMATION::KOOPA_WALK_LEFT;

    if (state == KOOPA_STATE_SHELL)
    {
        aniId = ANIMATION::KOOPA_SHELL;
    }
    else if (state == KOOPA_STATE_SPINNING)
    {
        aniId = ANIMATION::KOOPA_SPINNING;
    }
    else if (state == KOOPA_STATE_WALKING || state == KOOPA_STATE_DIE)
    {
        if (isFacingRight) aniId = ANIMATION::KOOPA_WALK_RIGHT;
        else aniId = ANIMATION::KOOPA_WALK_LEFT;
    }
    LPANIMATION ani = AnimationManager::GetInstance()->Get(aniId);
    if (ani != NULL)
    {
        ani->Render(x, y, z);
    }
}