#pragma once
#include "AnimationFrame.h"
#include <vector>
using namespace std;

class Animation
{
    vector<AnimationFrame*> frames;

    int currentFrame;
    DWORD lastFrameTime;

public:
    Animation()
    {
        currentFrame = -1;
        lastFrameTime = 0;
    }

    void Add(LPSPRITE sprite, DWORD time);
    void Render(float x, float y, float z);
    void Render(float x, float y, float z, bool flipX);

    ~Animation(){}
};
typedef Animation* LPANIMATION;

