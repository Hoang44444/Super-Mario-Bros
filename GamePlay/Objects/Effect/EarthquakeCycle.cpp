#include "EarthquakeCycle.h"

namespace
{
    constexpr ULONGLONG EARTHQUAKE_ACTIVE_MS   = 8000;
    constexpr ULONGLONG EARTHQUAKE_INACTIVE_MS = 10000;
}

EarthquakeCycle* EarthquakeCycle::__instance = nullptr;

EarthquakeCycle* EarthquakeCycle::GetInstance()
{
    if (__instance == nullptr)
        __instance = new EarthquakeCycle();
    return __instance;
}

void EarthquakeCycle::Start()
{
    enabled   = true;
    startTime = GetTickCount64();
}

void EarthquakeCycle::Stop()
{
    enabled = false;
}

bool EarthquakeCycle::IsActive() const
{
    if (!enabled) return false;

    ULONGLONG elapsed = GetTickCount64() - startTime;

    if (elapsed < EARTHQUAKE_INACTIVE_MS) return false;

    ULONGLONG pos = (elapsed - EARTHQUAKE_INACTIVE_MS) % (EARTHQUAKE_ACTIVE_MS + EARTHQUAKE_INACTIVE_MS);
    return pos < EARTHQUAKE_ACTIVE_MS;
}
