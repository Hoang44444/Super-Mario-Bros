#pragma once

#include "GameObject.h"

class Background : public GameObject
{
public:
    Background(float x, float y, float z) : GameObject(x, y, z) {}

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override {}
    void Render() override;

    void GetBoundingBox(float& l, float& t, float& r, float& b) override { l = t = r = b = 0; }

    bool IsCollidable() override { return false; }
    bool IsBlocking() override { return false; }
};
