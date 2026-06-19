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
    // Render the current frame stretched to an explicit destination size (used for
    // full-screen backgrounds that must be scaled to fit the logical screen).
    void RenderScaled(float x, float y, float z, float destWidth, float destHeight);

    ~Animation(){}
};
typedef Animation* LPANIMATION;

