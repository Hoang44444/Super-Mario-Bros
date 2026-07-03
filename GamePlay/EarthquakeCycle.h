#pragma once
#include <Windows.h>

// Trạng thái chu kỳ earthquake dùng cho level 1-4
// Earthquake xảy ra theo chu kỳ: nghỉ -> earthquake -> nghỉ -> earthquake...
class EarthquakeCycle
{
private:
    static EarthquakeCycle* __instance;

    bool      enabled   = false;   // màn hiện tại có earthquake hay không
    ULONGLONG startTime = 0;       // thời điểm bắt đầu chu kỳ (lúc vào màn)

    EarthquakeCycle() {}

public:
    static EarthquakeCycle* GetInstance();

    void Start();           // gọi khi vào màn có earthquake: bật và reset chu kỳ
    void Stop();            // tắt earthquake (các màn không có earthquake)
    bool IsActive() const;  // earthquake ĐANG xảy ra hay không
};
