#pragma once

#include "GameObject.h"

class MenuBackground : public GameObject
{
public:
    MenuBackground(float x, float y);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
    void Render() override;

    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    bool IsCollidable() override { return false; }
    bool IsBlocking() override { return false; }
};