#pragma once
#include <Windows.h>

class EarthquakeCycle
{
private:
    static EarthquakeCycle* __instance;

    bool      enabled   = false;
    ULONGLONG startTime = 0;

    EarthquakeCycle() {}

public:
    static EarthquakeCycle* GetInstance();

    void Start();
    void Stop();
    bool IsActive() const;
};
