#include "BowserFire.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"

BowserFire::BowserFire(float x, float y, float z, int direction) : Enemy(x, y, z)
{
    this->startX = x;
    this->direction = direction;
    this->vx = BOWSER_FIRE_SPEED * direction;
    this->vy = 0;
}

void BowserFire::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x; t = y; r = l + BOWSER_FIRE_BBOX_WIDTH; b = t + BOWSER_FIRE_BBOX_HEIGHT;
}

void BowserFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    x += vx * dt;

    float distanceTraveled = abs(this->x - startX);

    if (distanceTraveled > 400.0f)
    {
        this->Delete();
    }
}

void BowserFire::OnMarioCollison(Mario* mario, float ny)
{
    if (mario->GetState() != MARIO_STATE_DIE)
        mario->SetState(MARIO_STATE_DIE);
}

void BowserFire::Render()
{
    int aniId = (direction > 0) ? ANIMATION::BOWSER_FIRE_RIGHT : ANIMATION::BOWSER_FIRE_LEFT;
    LPANIMATION ani = AnimationManager::GetInstance()->Get(aniId);
    if (ani != nullptr) ani->Render(x, y, z);
}