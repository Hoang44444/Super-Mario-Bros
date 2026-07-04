#pragma once

#include "GameObject.h"

// Background - hình nền của scene
// Render hình nền tùy theo scene hiện tại (menu, level, death, ...)
class Background : public GameObject
{
public:
    Background(float x, float y, float z) : GameObject(x, y, z) {}
    virtual ~Background() {}    
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override {}  // Background tĩnh, không cần update
    void Render() override;

    void GetBoundingBox(float& l, float& t, float& r, float& b) override { l = t = r = b = 0; }  // Không có bounding box

    bool IsCollidable() override { return false; }  // Không va chạm
    bool IsBlocking() override { return false; }    // Không chặn

    int WorldRender();  // Trả về animation ID của background theo scene hiện tại
};
