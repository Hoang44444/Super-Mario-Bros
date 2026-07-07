#include "WindCycle.h"

namespace
{
    constexpr ULONGLONG WIND_ACTIVE_MS   = 5000;
    constexpr ULONGLONG WIND_INACTIVE_MS = 7000;
}

WindCycle* WindCycle::__instance = nullptr;

WindCycle* WindCycle::GetInstance()
{
    if (__instance == nullptr)
        __instance = new WindCycle();
    return __instance;
}

void WindCycle::Start()
{
    enabled   = true;
    startTime = GetTickCount64();
}

void WindCycle::Stop()
{
    enabled = false;
}

bool WindCycle::IsActive() const
{
    if (!enabled) return false;

    ULONGLONG elapsed = GetTickCount64() - startTime;

    if (elapsed < WIND_INACTIVE_MS) return false;

    ULONGLONG pos = (elapsed - WIND_INACTIVE_MS) % (WIND_ACTIVE_MS + WIND_INACTIVE_MS);
    return pos < WIND_ACTIVE_MS;
}
