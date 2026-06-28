#pragma once
#include "GameObject.h"

class WindEffect : public GameObject
{
private:
    float startY;
    float windSpeed;
    bool isWindActive = false;

public:
    WindEffect(float startX, float startY, float startZ = 0.0f);
    virtual ~WindEffect() {}

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
    virtual void Render() override;
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    virtual bool IsCollidable() override { return false; }
    virtual bool IsBlocking() override { return false; }
};