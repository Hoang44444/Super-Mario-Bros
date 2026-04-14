#include "Animation.h"
#include "Renderer.h"

void Animation::Add(LPSPRITE sprite, DWORD time)
{
    frames.push_back(new AnimationFrame(sprite, time));
}
void Animation::Render(float x, float y)
{
    if (frames.size() == 0) return;

    DWORD now = GetTickCount();

    if (currentFrame == -1)
    {
        currentFrame = 0;
        lastFrameTime = now;
    }
    else
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

    Renderer::GetInstance()->Draw(
        x, y,
        sprite->GetTexture(),
        &sprite->GetRect()
    );
}
