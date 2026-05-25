#pragma once
#include "GameObject.h"
#include "Mario.h"

class StaticObject : public GameObject{
public:
	StaticObject(float x, float y, float z) : GameObject(x, y, z){}
	~StaticObject(){}

	bool IsColliable() { return true; }
	bool IsBlocking() { return true; }

	virtual void OnMarioCollision(Mario* mario, LPCOLLISIONEVENT e);
};

