#include "Spiny.h"
#include "BuzzyBeetle.h"
#include "Koopa.h"
#include "Camera.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"
#include "Core/ScoreManager.h"

Spiny::Spiny(float x, float y, float z) : Enemy(x, y, z)
{
    bool isFacingRight;
    SetState(SPINY_STATE_WALKING);
}

void Spiny::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x;
    t = y;
    r = l + SPINY_BBOX_WIDTH;
    b = t + SPINY_BBOX_HEIGHT;
}

void Spiny::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (this->vx > 0) isFacingRight = true;
    else if (this->vx < 0) isFacingRight = false;

    if (state == SPINY_STATE_DIE)
    {
        vy += SPINY_GRAVITY * dt;

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

    vy += SPINY_GRAVITY * dt;


    float cx = Camera::GetInstance()->GetX();
    float cy = Camera::GetInstance()->GetY();

    const float SCREEN_WIDTH = 800.0f;
    const float SCREEN_HEIGHT = 600.0f;

    const float CULL_BUFFER = 100.0f;

    if (this->y > cy + SCREEN_HEIGHT + CULL_BUFFER ||
        this->x < cx - CULL_BUFFER ||
        this->x > cx + SCREEN_WIDTH + CULL_BUFFER)
    {
        this->Delete();
        return;
    }

    Collision::GetInstance()->Process(this, dt, coObjects);
}

bool Spiny::IsCollidable()
{
    if (state == SPINY_STATE_DIE) return false;
    return true;
}

void Spiny::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void Spiny::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<Koopa*>(e->obj))
    {
        Koopa* koopa = dynamic_cast<Koopa*>(e->obj);
        if (koopa->GetState() == KOOPA_STATE_SPINNING)
        {
            this->SetState(SPINY_STATE_DIE);
            // Award score for shell kill (no combo for shell kills)
            ScoreManager::Get().AddScore(SCORE_VALUES::SHELL_KICK);
            return;
        }
    }
    else if (dynamic_cast<BuzzyBeetle*>(e->obj))
    {
        BuzzyBeetle* beetle = dynamic_cast<BuzzyBeetle*>(e->obj);
        if (beetle->GetState() == BUZZY_STATE_SPINNING)
        {
            this->SetState(SPINY_STATE_DIE);
            // Award score for shell kill (no combo for shell kills)
            ScoreManager::Get().AddScore(SCORE_VALUES::SHELL_KICK);
            return;
        }
    }

    if (e->ny != 0 && e->obj->IsBlocking())
    {
        vy = 0;
        if (state == SPINY_STATE_EGG)
        {
            SetState(SPINY_STATE_WALKING);
        }
    }
    else if (e->nx != 0 && e->obj->IsBlocking())
    {
        if (state == SPINY_STATE_WALKING)
        {
            vx = -vx;
        }
    }
}

void Spiny::OnMarioCollison(Mario* mario, float ny)
{
    if (mario->GetState() == MARIO_STATE_DIE) return;

    if (state == SPINY_STATE_DIE) return;

    mario->TakeDamage();
}

void Spiny::SetState(int state)
{
    GameObject::SetState(state);
    switch (state)
    {
    case SPINY_STATE_WALKING:
        if (this->direction == 1)
        {
            vx = SPINY_WALK_SPEED;
        }
        else
        {
            vx = -SPINY_WALK_SPEED;
        }
        break;

    case SPINY_STATE_EGG:
        vy = -0.15f;
        vx = 0;
        break;
    case SPINY_STATE_DIE:
        vx = 0;
        vy = -0.3f;
        break;
    }
}

void Spiny::Render()
{
    int aniId = ANIMATION::SPINY_WALK_LEFT;

    if (state == SPINY_STATE_EGG)
    {
        aniId = ANIMATION::SPINY_EGG_SPIN;
    }

    else if (state == SPINY_STATE_WALKING || state == SPINY_STATE_DIE)
    {
        if (isFacingRight) aniId = ANIMATION::SPINY_WALK_RIGHT;
        else aniId = ANIMATION::SPINY_WALK_LEFT;
    }

    LPANIMATION ani = AnimationManager::GetInstance()->Get(aniId);
    if (ani != NULL)
    {
        ani->Render(x, y, z);
    }
}