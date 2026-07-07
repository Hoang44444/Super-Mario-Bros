#include "EarthquakeEffect.h"
#include <cmath>

namespace
{
    constexpr float QUAKE_SHAKE_MS    = 5000.0f;
    constexpr float QUAKE_INTERVAL_MS = 10000.0f;
    constexpr float QUAKE_AMPLITUDE_Y = 4.0f;
    constexpr float QUAKE_AMPLITUDE_X = 2.0f;
    constexpr float QUAKE_OMEGA_Y     = 0.060f;
    constexpr float QUAKE_OMEGA_X     = 0.045f;
}

void EarthquakeEffect::Start()
{
    enabled    = true;
    shaking    = false;
    phaseTimer = 0;
    elapsed    = 0;
    offsetX    = 0;
    offsetY    = 0;
}

void EarthquakeEffect::Stop()
{
    enabled    = false;
    shaking    = false;
    phaseTimer = 0;
    elapsed    = 0;
    offsetX    = 0;
    offsetY    = 0;
}

void EarthquakeEffect::Update(float dt)
{
    if (!enabled)
    {
        offsetX = offsetY = 0;
        return;
    }

    phaseTimer += dt;

    if (shaking)
    {
        elapsed += dt;

        if (phaseTimer >= QUAKE_SHAKE_MS)
        {
            shaking    = false;
            phaseTimer = 0;
            offsetX = offsetY = 0;
        }
        else
        {
            float k = 1.0f - phaseTimer / QUAKE_SHAKE_MS;
            offsetY = QUAKE_AMPLITUDE_Y * k * sinf(QUAKE_OMEGA_Y * elapsed);
            offsetX = QUAKE_AMPLITUDE_X * k * sinf(QUAKE_OMEGA_X * elapsed);
        }
    }
    else
    {
        offsetX = offsetY = 0;

        if (phaseTimer >= QUAKE_INTERVAL_MS)
        {
            shaking    = true;
            phaseTimer = 0;
            elapsed    = 0;
        }
    }
}
