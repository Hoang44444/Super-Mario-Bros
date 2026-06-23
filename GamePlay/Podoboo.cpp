#include "Podoboo.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "debug.h"

Podoboo::Podoboo(float x, float y, float z) : Enemy(x, y, z)
{
    this->startY = y;
    SetState(PODOBOO_STATE_HIDDEN);
}

void Podoboo::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    if (state == PODOBOO_STATE_HIDDEN) return;

    l = x;
    t = y;
    r = l + PODOBOO_BBOX_WIDTH;
    b = t + PODOBOO_BBOX_HEIGHT;
}

void Podoboo::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state == PODOBOO_STATE_HIDDEN)
    {
        if (GetTickCount64() - wait_start > PODOBOO_WAIT_TIME)
        {
            SetState(PODOBOO_STATE_JUMPING);
        }
    }
    else if (state == PODOBOO_STATE_JUMPING)
    {
        vy += PODOBOO_GRAVITY * dt;

        y += vy * dt;

        if (vy > 0 && y >= startY)
        {
            y = startY;
            SetState(PODOBOO_STATE_HIDDEN);
        }
    }

}

void Podoboo::OnMarioCollison(Mario* mario, float ny)
{
    if (mario->GetState() == MARIO_STATE_DIE) return;
    if (state == PODOBOO_STATE_HIDDEN) return;

    mario->TakeDamage();
}

void Podoboo::SetState(int state)
{
    GameObject::SetState(state);
    switch (state)
    {
    case PODOBOO_STATE_HIDDEN:
        vy = 0;
        vx = 0;
        wait_start = GetTickCount64();
        break;
    case PODOBOO_STATE_JUMPING:
        vy = -PODOBOO_JUMP_SPEED;
        break;
    }
}

void Podoboo::Render()
{
    if (state == PODOBOO_STATE_HIDDEN) return;

    int aniId = (vy < 0) ? ANIMATION::PODOBOO_UP : ANIMATION::PODOBOO_DOWN;

    AnimationManager::GetInstance()->Get(aniId)->Render(x, y, z);
}