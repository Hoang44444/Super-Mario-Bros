#include "EarthquakeCycle.h"

namespace
{
    constexpr ULONGLONG EARTHQUAKE_ACTIVE_MS   = 8000;    // mỗi đợt earthquake kéo dài 8 giây
    constexpr ULONGLONG EARTHQUAKE_INACTIVE_MS = 10000;   // nghỉ 10 giây giữa các đợt (và trước đợt đầu)
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

    // Chu kỳ nghỉ đầu tiên: chưa có earthquake.
    if (elapsed < EARTHQUAKE_INACTIVE_MS) return false;

    // Sau đó lặp: EARTHQUAKE 8s -> NGHỈ 10s -> EARTHQUAKE 8s -> ...
    ULONGLONG pos = (elapsed - EARTHQUAKE_INACTIVE_MS) % (EARTHQUAKE_ACTIVE_MS + EARTHQUAKE_INACTIVE_MS);
    return pos < EARTHQUAKE_ACTIVE_MS;
}
