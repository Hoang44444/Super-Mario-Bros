#include "WindCycle.h"

namespace
{
    constexpr ULONGLONG WIND_ACTIVE_MS   = 5000;    // mỗi đợt gió thổi 5 giây
    constexpr ULONGLONG WIND_INACTIVE_MS = 7000;    // nghỉ 7 giây giữa các đợt (và trước đợt đầu)
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

    // Chu kỳ nghỉ đầu tiên: chưa có gió.
    if (elapsed < WIND_INACTIVE_MS) return false;

    // Sau đó lặp: THỔI 5s -> NGHỈ 7s -> THỔI 5s -> ...
    ULONGLONG pos = (elapsed - WIND_INACTIVE_MS) % (WIND_ACTIVE_MS + WIND_INACTIVE_MS);
    return pos < WIND_ACTIVE_MS;
}
