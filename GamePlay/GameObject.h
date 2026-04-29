#pragma once
#include <vector>
#include <Windows.h>
#include "Collision.h"
#include "Scene.h"

using namespace std;


class CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;

class GameObject
{
protected:
    float x, y;
    float vx, vy;
    int direction; // -1: left, 1: right
    int state;
    bool isDeleted;
	Scene* scene; // Reference to the scene the object belongs to

public:
    GameObject() {
        this->x = this->y = 0;
        this->vx = this->vy = 0;
        this->direction = 1;
        this->state = -1;
        this->isDeleted = false;
    };
    GameObject(float x, float y) : GameObject() { this->x = x; this->y = y;};

    ~GameObject() {};

    // Position
    void SetPosition(float x, float y) { this->x = x; this->y = y;};
    void GetPosition(float& x, float& y) { x = this->x; y = this->y; };

    // Speed
    void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; };
    void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; };

    int GetState() { return this->state; };
    virtual void SetState(int state) { this->state = state; };

    virtual void Delete() { this->isDeleted = true; };
    bool IsDeleted() { return this->isDeleted; };

    // Core
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) = 0;
    virtual void Render() = 0;

    // Collision
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b) = 0;
    virtual bool IsCollidable() { return false; }
    virtual bool IsBlocking() { return true; }

    virtual void OnNoCollision(DWORD dt) {}
    virtual void OnCollisionWith(LPCOLLISIONEVENT e) {}

    static bool IsDeleted(const LPGAMEOBJECT& o) { return o->isDeleted; };

	// Scene management
	void SetScene(Scene* s) { this->scene = s; }
};
typedef GameObject* LPGAMEOBJECT;
