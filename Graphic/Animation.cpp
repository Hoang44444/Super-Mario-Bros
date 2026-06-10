#include "Animation.h"
#include "Renderer.h"

void Animation::Add(LPSPRITE sprite, DWORD time)
{
    frames.push_back(new AnimationFrame(sprite, time));
}

void Animation::Render(float x, float y, float z)
{
    Render(x, y, z, false);
}

void Animation::Render(float x, float y, float z, bool flipX)
{
    if (frames.size() == 0) return;

    DWORD now = GetTickCount64();

    if (currentFrame == -1)
    {
        currentFrame = 0;
        lastFrameTime = now;
    }
    else if (frames.size() > 1) 
    {
        DWORD t = frames[currentFrame]->GetTime();
        if (now - lastFrameTime > t)
        {
            currentFrame++;
            if (currentFrame >= frames.size())
                currentFrame = 0;

            lastFrameTime = now;
        }
    }

    LPSPRITE sprite = frames[currentFrame]->GetSprite();
    if (sprite == nullptr) return;

    RECT rect = sprite->GetRect();
    if (sprite->GetTexture() == nullptr) return;

    Renderer::GetInstance()->Draw(
        x, y, z,
        sprite->GetTexture(),
        &rect,
        1.0f,
        flipX
    );
}