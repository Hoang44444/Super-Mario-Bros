#pragma once

class EarthquakeEffect
{
private:
    bool  enabled    = false;
    bool  shaking    = false;
    float phaseTimer = 0;
    float elapsed    = 0;
    float offsetX    = 0;
    float offsetY    = 0;

public:
    void Start();

    void Stop();

    void Update(float dt);

    bool  IsActive()   const { return enabled; }
    bool  IsShaking()  const { return shaking; }
    float GetOffsetX() const { return offsetX; }
    float GetOffsetY() const { return offsetY; }
};
