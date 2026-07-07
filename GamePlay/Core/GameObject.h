#pragma once
#include <vector>
#include <Windows.h>
#include "Collision.h"
#include "Scene.h"

using namespace std;


struct CollisionEvent;
typedef CollisionEvent* LPCOLLISIONEVENT;

class GameObject
{
protected:
    float x, y, z;
    float vx, vy;
    int direction;
    int state;
    bool isDeleted;
	bool isVisible;
	Scene* scene;

public:
    GameObject() {
        this->x = this->y = this->z = 0;
        this->vx = this->vy = 0;
        this->direction = 1;
        this->state = -1;
        this->isDeleted = false;
		this->isVisible = true;
		scene = nullptr;
    };

    GameObject(float x, float y, float z) : GameObject() { this->x = x; this->y = y; this->z = z; };

    virtual ~GameObject() {};

    void SetPosition(float x, float y, float z) { this->x = x; this->y = y; this->z = z; };
    void GetPosition(float& x, float& y, float& z) { x = this->x; y = this->y; z = this->z; };

    void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; };
    void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; };

    int GetState() { return this->state; };
    virtual void SetState(int state) { this->state = state; };

    virtual void Delete() { this->isDeleted = true; };
    bool IsDeleted() { return this->isDeleted; };

    void SetVisible(bool visible) { this->isVisible = visible; }
    bool IsVisible() { return this->isVisible; }

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) = 0;

    virtual void Render() = 0;

    virtual void GetBoundingBox(float& l, float& t, float& r, float& b) = 0;

    virtual bool IsCollidable() { return false; }

    virtual bool IsBlocking() { return true; }

    virtual void OnNoCollision(DWORD dt) {}

    virtual void OnCollisionWith(LPCOLLISIONEVENT e) {}

    virtual void StartEmerge() {}

    static bool IsDeleted(const LPGAMEOBJECT& o) { return o->isDeleted; };

    virtual int IsDirectionColliable(float nx, float ny) { return 1; }

	void SetScene(Scene* s) { this->scene = s; }
};
typedef GameObject* LPGAMEOBJECT;
