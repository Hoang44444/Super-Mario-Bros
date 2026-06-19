#include "BuzzyBeetle.h"
#include "Koopa.h"
#include "Camera.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"

BuzzyBeetle::BuzzyBeetle(float x, float y, float z) : Enemy(x, y, z)
{
    bool isFacingRight;
    SetState(BUZZY_STATE_WALKING);
}

void BuzzyBeetle::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x;
    t = y;
    r = l + BUZZY_BBOX_WIDTH;

    if (state == BUZZY_STATE_WALKING)
        b = t + BUZZY_BBOX_HEIGHT;
    else
        b = t + BUZZY_BBOX_HEIGHT_SHELL;
}

void BuzzyBeetle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (this->vx > 0) isFacingRight = true;
    else if (this->vx < 0) isFacingRight = false;

    if (state == BUZZY_STATE_DIE)
    {
        vy += BUZZY_GRAVITY * dt;

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

    vy += BUZZY_GRAVITY * dt;

    if (state == BUZZY_STATE_SHELL && GetTickCount64() - shell_start > BUZZY_SHELL_TIMEOUT)
    {
        SetState(BUZZY_STATE_WALKING);

        y -= (BUZZY_BBOX_HEIGHT - BUZZY_BBOX_HEIGHT_SHELL);
    }

    Collision::GetInstance()->Process(this, dt, coObjects);
}

bool BuzzyBeetle::IsCollidable()
{
    if (state == KOOPA_STATE_DIE) return false;
    return true;
}

void BuzzyBeetle::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void BuzzyBeetle::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<Koopa*>(e->obj))
    {
        Koopa* koopa = dynamic_cast<Koopa*>(e->obj);
        if (koopa->GetState() == KOOPA_STATE_SPINNING)
        {
            this->SetState(BUZZY_STATE_DIE);
            return;
        }
    }
    else if (dynamic_cast<BuzzyBeetle*>(e->obj))
    {
        BuzzyBeetle* beetle = dynamic_cast<BuzzyBeetle*>(e->obj);
        if (beetle->GetState() == BUZZY_STATE_SPINNING)
        {
            this->SetState(BUZZY_STATE_DIE);
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

void BuzzyBeetle::OnMarioCollison(Mario* mario, float ny)
{
    if (mario->GetState() == MARIO_STATE_DIE) return;

    if (state == BUZZY_STATE_DIE) return;

    float mx, my, mz;
    mario->GetPosition(mx, my, mz);
    float mvx, mvy;
    mario->GetSpeed(mvx, mvy);

    if (state == BUZZY_STATE_WALKING)
    {
        if (ny < 0)
        {
            SetState(BUZZY_STATE_SHELL);
            mario->SetSpeed(mvx, -0.4f);
        }
        else
        {
            mario->TakeDamage();
        }
    }
    else if (state == BUZZY_STATE_SHELL)
    {
        if (mx < this->x)
            vx = BUZZY_SPIN_SPEED;
        else
            vx = -BUZZY_SPIN_SPEED;

        SetState(BUZZY_STATE_SPINNING);

        if (ny < 0) mario->SetSpeed(mvx, -0.4f);
    }
    else if (state == BUZZY_STATE_SPINNING)
    {
        if (ny < 0)
        {
            SetState(BUZZY_STATE_SHELL);
            mario->SetSpeed(mvx, -0.4f);
        }
        else
        {
            mario->TakeDamage();
        }
    }
}

void BuzzyBeetle::SetState(int state)
{
    int oldState = this->state;
    GameObject::SetState(state);

    switch (state)
    {
    case BUZZY_STATE_WALKING:
        vx = -BUZZY_WALK_SPEED;
        vy = 0;
        break;
    case BUZZY_STATE_SHELL:
        shell_start = GetTickCount64();

        vx = 0;
        vy = 0;
        if (oldState == BUZZY_STATE_WALKING)
        {
            y += (BUZZY_BBOX_HEIGHT - BUZZY_BBOX_HEIGHT_SHELL);
        }
        break;
    case BUZZY_STATE_SPINNING:
        break;
    case BUZZY_STATE_DIE:
        vx = 0;
        vy = -0.3f;
        break;
    }
}

void BuzzyBeetle::Render()
{
    int aniId = ANIMATION::BUZZY_WALK_LEFT;

    if (state == BUZZY_STATE_SHELL)
    {
        aniId = ANIMATION::BUZZY_SHELL;
    }
    else if (state == BUZZY_STATE_SPINNING)
    {
        aniId = ANIMATION::BUZZY_SPINNING;
    }
    else if (state == BUZZY_STATE_WALKING || state == BUZZY_STATE_DIE)
    {
        if (isFacingRight) aniId = ANIMATION::BUZZY_WALK_RIGHT;
        else aniId = ANIMATION::BUZZY_WALK_LEFT;
    }

    AnimationManager::GetInstance()->Get(aniId)->Render(x, y, z);
}