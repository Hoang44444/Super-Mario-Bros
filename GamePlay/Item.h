#pragma once
#include "GameObject.h"
class Item : public GameObject{
public: 
	Item(float x, float y, float z) : GameObject(x, y, z) {}
	~Item() {}

	bool IsCollidable() { return true; }
	bool IsBlocking() { return false; }	
};

