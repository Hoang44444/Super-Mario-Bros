#pragma once
#include "Item.h"
#include "Collision.h"

#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_SPEED 0.05f
#define MUSHROOM_BBOX_WIDTH 16.0f
#define MUSHROOM_BBOX_HEIGHT 16.0f

// Mushroom base (nấm đỏ lớn, di chuyển ngang)
class Mushroom : public Item {
protected:
    float ay;
public:
    Mushroom(float x, float y, float z) : Item(x, y, z) {
        vx = MUSHROOM_SPEED; // di chuyển sang phải mặc định
        ay = MUSHROOM_GRAVITY;
    }
    virtual ~Mushroom() {}

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void OnNoCollision(DWORD dt) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

    // Nấm mặc định: biến Mario_Small thành Mario_Big
    void OnMarioCollision(Mario* mario) override;
};
