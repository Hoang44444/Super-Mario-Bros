#include "WindEffect.h"
#include "Camera.h"
#include "AnimationManager.h"
#include "AssetID.h"
#include "WindCycle.h"

namespace
{
    constexpr float WIND_SPAWN_INSET  = 0.0f;
    constexpr float WIND_DESPAWN_LEFT = 50.0f;
    constexpr float WIND_WRAP_BOTTOM  = 20.0f;
    constexpr float WIND_WRAP_TOP     = 20.0f;
    constexpr int   WIND_SPAWN_Y_RANGE = 150;

    constexpr float WIND_FLUTTER_FREQ = 0.005f;
    constexpr float WIND_FLUTTER_AMP  = 0.05f;
}

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
    this->isWindActive = WindCycle::GetInstance()->IsActive();

    float cx = Camera::GetInstance()->GetX();
    float cy = Camera::GetInstance()->GetY();

    int screenW = 0, screenH = 0;
    Camera::GetInstance()->GetSize(screenW, screenH);

    float spawnX = cx + screenW - WIND_SPAWN_INSET;

    if (this->isWindActive)
    {
        float time = (float)GetTickCount64();
        float flutter = sin(time * WIND_FLUTTER_FREQ) * WIND_FLUTTER_AMP;

        this->x += vx * dt;
        this->y += (vy + flutter) * dt;

        if (this->x < cx - WIND_DESPAWN_LEFT)
        {
            this->x = spawnX;
            this->y = cy + static_cast<float>(rand() % WIND_SPAWN_Y_RANGE);
        }
        if (this->y > cy + screenH + WIND_WRAP_BOTTOM)
        {
            this->y = cy - WIND_WRAP_TOP;
        }
    }
    else
    {
        this->x = spawnX;
    }
}

void WindEffect::Render()
{
    if (!this->isWindActive) return;
    AnimationManager::GetInstance()->Get(ANIMATION::WIND_EFFECT)->Render(x, y, z);
}
