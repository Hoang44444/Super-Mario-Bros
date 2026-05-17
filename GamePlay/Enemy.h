#pragma once
#include "GameObject.h"
class Enemy : public GameObject{
public:
	Enemy(float x, float y, float z) : GameObject(x, y, z) {};
	~Enemy() {};

	bool IsCollidable() { return  true; }
	bool IsBlocking() { return false; }
};

