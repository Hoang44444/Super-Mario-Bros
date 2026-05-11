#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>
#include <cmath>

#include "../Graphic/Animation.h"
#include "../Graphic/AnimationManager.h"
#include "../Resource/SpriteManager.h"
#include "Collision.h"
#include "Scene.h"

using namespace std;

#define ID_TEX_BBOX -100

class GameObject;
typedef GameObject* LPGAMEOBJECT;

class GameObject
{
protected:
	float x, y;
	float vx, vy;
	float dx, dy;
	int direction; // -1: left, 1: right
	int state;
	bool isDeleted;
	Scene* scene;

public:
	GameObject() {
		this->x = this->y = 0;
		this->vx = this->vy = 0;
		this->dx = this->dy = 0;
		this->direction = 1;
		this->state = -1;
		this->isDeleted = false;
		scene = nullptr;
	}

	GameObject(float x, float y) : GameObject() {
		this->x = x;
		this->y = y;
	}
    virtual ~GameObject() {};

	// --- POSITION & GETTERS (Fixed C2248) ---
	void SetPosition(float x, float y) { this->x = x; this->y = y; }
	void GetPosition(float& x, float& y) const { x = this->x; y = this->y; }

	// Add these to allow Mario to "see" Troopa's position
	float GetX() const { return x; }
	float GetY() const { return y; }

	// --- SPEED & DIRECTION ---
	void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; }
	void GetSpeed(float& vx, float& vy) const { vx = this->vx; vy = this->vy; }

	float GetVx() const { return vx; }
	float GetVy() const { return vy; }

	int GetDirection() const { return direction; }
	void SetDirection(int d) { direction = d; }

	// --- STATE & SCENE ---
	int GetState() const { return this->state; }
	virtual void SetState(int state) { this->state = state; }
	void SetScene(Scene* s) { this->scene = s; }

	// --- DELETION ---
	virtual void Delete() { isDeleted = true; }
	bool IsDeleted() const { return isDeleted; }
	static bool IsDeleted(const LPGAMEOBJECT& o) { return o->isDeleted; }

	// --- CORE VIRTUALS ---
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {}
	virtual void Render() = 0;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;

	void RenderBoundingBox();

	// --- COLLISION PROPERTIES ---
	virtual bool IsCollidable() { return false; }
	virtual bool IsBlocking() { return true; }
	virtual int IsDirectionColliable(float nx, float ny) { return 1; }

	// --- COLLISION EVENTS ---
	virtual void OnNoCollision(DWORD dt) {}
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {}
};