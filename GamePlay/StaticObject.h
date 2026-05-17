#pragma once
#include "GameObject.h"

class StaticObject : public GameObject{
public:
	StaticObject(float x, float y, float z) : GameObject(x, y, z){}
	~StaticObject(){}

	bool IsColliable() { return true; }
	bool IsBlocking() { return true; }
};

