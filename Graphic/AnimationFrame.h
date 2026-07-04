#pragma once
#include "Sprite.h"

// AnimationFrame đại diện cho một frame trong chuỗi animation
// Mỗi frame có một sprite và thời gian hiển thị (ms)
class AnimationFrame
{
    LPSPRITE sprite;  // Sprite của frame này
    DWORD time;       // Thời gian hiển thị frame (ms)

public:
    AnimationFrame(LPSPRITE sprite, DWORD time)
    {
        this->sprite = sprite;
        this->time = time;
    }

    LPSPRITE GetSprite() { return sprite; }
    DWORD GetTime() { return time; }
};
