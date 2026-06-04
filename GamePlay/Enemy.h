#pragma once
#include "GameObject.h"
#include "Mario.h"
class Enemy : public GameObject{
public:
	Enemy(float x, float y, float z) : GameObject(x, y, z) {};
	virtual ~Enemy() {};

	bool IsCollidable() { return  true; }
	bool IsBlocking() { return false; }

	virtual void OnMarioCollison(Mario* mario) = 0; // Define this in derived classes to specify what happens when Mario collides with the enemy
};

