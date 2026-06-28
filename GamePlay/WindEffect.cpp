#include "WindEffect.h"
#include "Camera.h"
#include "AnimationManager.h"
#include "AssetID.h"

WindEffect::WindEffect(float startX, float startY, float startZ) : GameObject(startX, startY, startZ)
{
    this->windSpeed = -(0.15f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.1f)));
    this->vx = this->windSpeed;

    this->vy = 0.02f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.03f));

    this->startY = startY;
    this->direction = -1;
}

void WindEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x;
    t = y;
    r = x + 8.0f;
    b = y + 8.0f;
}

void WindEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    ULONGLONG now = GetTickCount64();
    this->isWindActive = (now % 10000) < 5000;

    float cx = Camera::GetInstance()->GetX();
    float cy = Camera::GetInstance()->GetY();
    
    if (this->isWindActive)
    {
        float time = (float)now;
        float flutter = sin(time * 0.005f) * 0.05f;

        this->x += vx * dt;
        this->y += (vy + flutter) * dt;

        if (this->x < cx - 50.0f)
        {
            this->x = cx + 320.0f + 50.0f;
            this->y = cy + static_cast<float>(rand() % 150);
        }
        if (this->y > cy + 240.0f + 20.0f)
        {
            this->y = cy - 20.0f;
        }
    }
    else
    {
        this->x = cx + 320.0f + 50.0f;
    }
}

void WindEffect::Render()
{
    if (!this->isWindActive) return;
    AnimationManager::GetInstance()->Get(ANIMATION::WIND_EFFECT)->Render(x, y, z);
}