#include "Lakitu.h"
#include "Spiny.h"
#include "BuzzyBeetle.h"
#include "Koopa.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Camera.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "Core/ScoreManager.h"

Lakitu::Lakitu(float x, float y, float z) : Enemy(x, y, z)
{
    bool isFacingRight;
    this->startX = x;
    SetState(LAKITU_STATE_FLYING);
    throw_start = GetTickCount64();
}

void Lakitu::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x; t = y; r = l + LAKITU_BBOX_WIDTH; b = t + LAKITU_BBOX_HEIGHT;
}

void Lakitu::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (this->vx > 0) isFacingRight = true;
    else if (this->vx < 0) isFacingRight = false;

    if (state == LAKITU_STATE_DIE)
    {
        vy += LAKITU_GRAVITY * dt;
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

    Mario* mario = NULL;

    for (size_t i = 0; i < coObjects->size(); i++)
    {
        if (dynamic_cast<Mario*>(coObjects->at(i)))
        {
            mario = dynamic_cast<Mario*>(coObjects->at(i));
            break;
        }
    }

    if (mario != NULL && mario->GetState() != MARIO_STATE_DIE)
    {
        float mx, my, mz;
        mario->GetPosition(mx, my, mz);

        const float HOVER_RADIUS = 50.0f;

        if (vx > 0 && this->x > mx + HOVER_RADIUS)
        {
            vx = -LAKITU_FLY_SPEED;
        }
        else if (vx < 0 && this->x < mx - HOVER_RADIUS)
        {
            vx = LAKITU_FLY_SPEED;
        }
    }

    x += vx * dt;

    if (GetTickCount64() - throw_start > LAKITU_THROW_COOLDOWN)
    {
        throw_start = GetTickCount64();

        float cx = Camera::GetInstance()->GetX();
        float cy = Camera::GetInstance()->GetY();

        const float SCREEN_WIDTH = 800.0f;

        int activeSpinies = 0;
        for (size_t i = 0; i < coObjects->size(); i++)
        {
            Spiny* spiny = dynamic_cast<Spiny*>(coObjects->at(i));

            if (spiny != NULL && !spiny->IsDeleted())
            {
                float sx, sy, sz;
                spiny->GetPosition(sx, sy, sz);

                if (sx >= cx && sx <= cx + SCREEN_WIDTH)
                {
                    activeSpinies++;
                }
            }
        }

        if (activeSpinies < 4)
        {
            Spiny* egg = new Spiny(this->x, this->y, this->z);
            egg->SetState(SPINY_STATE_EGG);

            int lakituDirection = (this->vx > 0) ? 1 : -1;
            egg->SetFacingDirection(lakituDirection);

            PlayScene* currentScene = dynamic_cast<PlayScene*>(this->scene);
            if (currentScene != NULL)
            {
                currentScene->AddObject(egg);
            }
        }
    }
}

bool Lakitu::IsCollidable()
{
    if (state == LAKITU_STATE_DIE) return false;
    return true;
}

void Lakitu::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<Koopa*>(e->obj))
    {
        Koopa* koopa = dynamic_cast<Koopa*>(e->obj);
        if (koopa->GetState() == KOOPA_STATE_SPINNING)
        {
            this->SetState(LAKITU_STATE_DIE);
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
            this->SetState(LAKITU_STATE_DIE);
            // Award score for shell kill (no combo for shell kills)
            ScoreManager::Get().AddScore(SCORE_VALUES::SHELL_KICK);
            return;
        }
    }
}

void Lakitu::OnMarioCollison(Mario* mario, float ny)
{
    if (mario->GetState() == MARIO_STATE_DIE) return;

    if (state == LAKITU_STATE_FLYING)
    {
        if (ny < 0)
        {
            SetState(LAKITU_STATE_DIE);
            float mx, my, mz, mvx, mvy;
            mario->GetSpeed(mvx, mvy);
            mario->SetSpeed(mvx, -0.4f);

            // Award score with combo system for stomp
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
}

void Lakitu::SetState(int state)
{
    GameObject::SetState(state);
    switch (state)
    {
    case LAKITU_STATE_FLYING:
        vx = -LAKITU_FLY_SPEED;
        vy = 0;
        break;
    case LAKITU_STATE_DIE:
        vx = 0;
        vy = 0.2f;
        break;
    }
}

void Lakitu::Render()
{
    int aniId = ANIMATION::LAKITU_FLY_RIGHT;
    if (state == LAKITU_STATE_FLYING || state == LAKITU_STATE_DIE)
    {
        if (isFacingRight) aniId = ANIMATION::LAKITU_FLY_RIGHT;
        else aniId = ANIMATION::LAKITU_FLY_LEFT;
    }
    
    LPANIMATION ani = AnimationManager::GetInstance()->Get(aniId);
    if (ani != NULL) ani->Render(x, y, z);
}