#pragma once
#include "GameObject.h"
#include "Mario.h"
class InvisibleObject : public GameObject{
public:
	InvisibleObject(float x, float y, float z) : GameObject(x, y, z) {};
	virtual ~InvisibleObject() {};

	virtual void OnMarioCollision(Mario* mario) {};

	bool IsCollidable() { return true; }
	bool IsBlocking() { return false; }


};

