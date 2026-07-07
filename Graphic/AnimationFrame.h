#pragma once
#include "Sprite.h"

class AnimationFrame
{
    LPSPRITE sprite;
    DWORD time;

public:
    AnimationFrame(LPSPRITE sprite, DWORD time)
    {
        this->sprite = sprite;
        this->time = time;
    }

    LPSPRITE GetSprite() { return sprite; }
    DWORD GetTime() { return time; }
};
