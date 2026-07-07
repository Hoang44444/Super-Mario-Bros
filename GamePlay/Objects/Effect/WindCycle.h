#pragma once
#include <Windows.h>

class WindCycle
{
private:
    static WindCycle* __instance;

    bool      enabled   = false;
    ULONGLONG startTime = 0;

    WindCycle() {}

public:
    static WindCycle* GetInstance();

    void Start();
    void Stop();
    bool IsActive() const;
};
