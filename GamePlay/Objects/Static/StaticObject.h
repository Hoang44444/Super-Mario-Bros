#pragma once
#include "GameObject.h"
#include "Mario.h"

// Base class cho các vật tĩnh (gạch, đất, pipe, ...)
// Không di chuyển, chặn Mario và các đối tượng khác
class StaticObject : public GameObject
{
public:
	StaticObject(float x, float y, float z) : GameObject(x, y, z){}
	~StaticObject(){}

	bool IsColliable() { return true; }
	bool IsBlocking() { return true; }

	// Xử lý khi Mario va chạm với vật tĩnh (dừng tốc độ theo hướng va chạm)
	virtual void OnMarioCollision(Mario* mario, LPCOLLISIONEVENT e);
};

