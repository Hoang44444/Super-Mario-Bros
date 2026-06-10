#include "Mushroom.h"
#include "StaticObject.h"
#include "Mario.h"

void Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    vy += ay * dt;
    Collision::GetInstance()->Process(this, dt, coObjects);
}

void Mushroom::OnNoCollision(DWORD dt) {
    x += vx * dt;
    y += vy * dt;
}

void Mushroom::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x - MUSHROOM_BBOX_WIDTH / 2;
    t = y - MUSHROOM_BBOX_HEIGHT / 2;
    r = l + MUSHROOM_BBOX_WIDTH;
    b = t + MUSHROOM_BBOX_HEIGHT;
}

void Mushroom::Render() {
}

void Mushroom::OnCollisionWith(LPCOLLISIONEVENT e) {
    if (dynamic_cast<StaticObject*>(e->obj)) {
        if (e->nx != 0) {
            vx = -vx;
        }
        if (e->ny < 0) {
            vy = 0;
        }
    }
}

void Mushroom::OnMarioCollision(Mario* mario) {
    if (!IsDeleted()) {
        mario->SetLevel(MARIO_LEVEL_BIG);
        Delete();
    }
}
