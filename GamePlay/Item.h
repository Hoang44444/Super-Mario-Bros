#pragma once
#include "GameObject.h"
#include "Mario.h"
class Item : public GameObject{
public: 
	Item(float x, float y, float z) : GameObject(x, y, z) {}
	virtual ~Item() {}

	bool IsCollidable() { return true; }
	bool IsBlocking() { return false; }

	virtual void OnMarioCollision(Mario * mario) = 0; // Define this in derived classes to specify what happens when Mario collides with the item
};

